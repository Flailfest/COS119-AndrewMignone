#include "ConsoleCentering.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ostream>
#include <streambuf>
#include <string>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace
{
    int getConsoleWidth()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        if (output != INVALID_HANDLE_VALUE &&
            GetConsoleScreenBufferInfo(output, &info))
        {
            const int width = info.srWindow.Right - info.srWindow.Left + 1;
            if (width > 0)
            {
                return width;
            }
        }
#else
        struct winsize sizeInfo;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizeInfo) == 0 &&
            sizeInfo.ws_col > 0)
        {
            return static_cast<int>(sizeInfo.ws_col);
        }
#endif
        // Sensible fallback for consoles where width detection is unavailable.
        return 100;
    }

    bool isAnsiSequenceStart(const std::string& text, std::size_t index)
    {
        return index + 1 < text.size() &&
            static_cast<unsigned char>(text[index]) == 0x1B &&
            text[index + 1] == '[';
    }

    std::string visibleText(const std::string& text)
    {
        std::string result;
        result.reserve(text.size());

        for (std::size_t i = 0; i < text.size();)
        {
            if (isAnsiSequenceStart(text, i))
            {
                i += 2;
                while (i < text.size())
                {
                    const unsigned char ch =
                        static_cast<unsigned char>(text[i++]);
                    // ANSI CSI sequences end with a byte in the 0x40-0x7E range.
                    if (ch >= 0x40 && ch <= 0x7E)
                    {
                        break;
                    }
                }
                continue;
            }

            result += text[i++];
        }

        return result;
    }

    int visibleWidth(const std::string& text)
    {
        const std::string visible = visibleText(text);
        int width = 0;

        for (std::size_t i = 0; i < visible.size(); ++i)
        {
            const unsigned char ch =
                static_cast<unsigned char>(visible[i]);
            if ((ch & 0xC0) != 0x80) // Count a UTF-8 code point once.
            {
                width += (ch == '\t') ? 4 : 1;
            }
        }

        return width;
    }

    bool looksLikeLayoutLine(const std::string& text)
    {
        const std::string visible = visibleText(text);
        int symbols = 0;
        int nonSpaces = 0;

        for (std::size_t i = 0; i < visible.size(); ++i)
        {
            const unsigned char ch =
                static_cast<unsigned char>(visible[i]);
            if (std::isspace(ch))
            {
                continue;
            }

            ++nonSpaces;
            if (std::string("|/\\+-_=~[]{}<>#*.:`'\"^").find(
                    static_cast<char>(ch)) != std::string::npos)
            {
                ++symbols;
            }
        }

        // Preserve the internal spacing in ASCII art and map rows.
        return symbols >= 2 && nonSpaces > 0 && symbols * 2 >= nonSpaces;
    }

    struct Token
    {
        std::string bytes;
        bool ansi;
        bool whitespace;
    };

    std::vector<Token> tokenize(const std::string& text)
    {
        std::vector<Token> tokens;
        for (std::size_t i = 0; i < text.size();)
        {
            if (isAnsiSequenceStart(text, i))
            {
                const std::size_t start = i;
                i += 2;
                while (i < text.size())
                {
                    const unsigned char ch =
                        static_cast<unsigned char>(text[i++]);
                    if (ch >= 0x40 && ch <= 0x7E)
                    {
                        break;
                    }
                }
                tokens.push_back(Token{text.substr(start, i - start), true, false});
            }
            else
            {
                const char ch = text[i++];
                tokens.push_back(Token{
                    std::string(1, ch),
                    false,
                    ch == ' ' || ch == '\t' || ch == '\r'
                });
            }
        }
        return tokens;
    }

    std::string trimTextIndent(const std::string& text)
    {
        const std::vector<Token> tokens = tokenize(text);
        std::size_t firstVisible = tokens.size();
        std::size_t lastVisible = 0;

        for (std::size_t i = 0; i < tokens.size(); ++i)
        {
            if (!tokens[i].ansi && !tokens[i].whitespace)
            {
                if (firstVisible == tokens.size())
                {
                    firstVisible = i;
                }
                lastVisible = i;
            }
        }

        if (firstVisible == tokens.size())
        {
            // Keep escape sequences (for example a color reset), but drop blank padding.
            std::string escapesOnly;
            for (std::size_t i = 0; i < tokens.size(); ++i)
            {
                if (tokens[i].ansi)
                {
                    escapesOnly += tokens[i].bytes;
                }
            }
            return escapesOnly;
        }

        // Keep ANSI controls wherever they occur, while removing only visible edge padding.
        std::string result;
        bool seenContent = false;
        for (std::size_t i = 0; i < tokens.size(); ++i)
        {
            if (tokens[i].ansi)
            {
                // Preserve ANSI styling controls, including reset sequences at line end.
                result += tokens[i].bytes;
                continue;
            }

            if (!seenContent && tokens[i].whitespace)
            {
                continue;
            }

            seenContent = true;
            if (i > lastVisible && tokens[i].whitespace)
            {
                continue;
            }
            result += tokens[i].bytes;
        }
        return result;
    }

    std::string prepareLine(const std::string& line)
    {
        if (looksLikeLayoutLine(line))
        {
            return line;
        }
        return trimTextIndent(line);
    }

    class CenteredBuffer : public std::streambuf
    {
    public:
        explicit CenteredBuffer(std::streambuf* destination)
            : destination_(destination)
        {
        }

    protected:
        int_type overflow(int_type ch) override
        {
            if (traits_type::eq_int_type(ch, traits_type::eof()))
            {
                return traits_type::not_eof(ch);
            }

            const char character = traits_type::to_char_type(ch);
            if (character == '\n')
            {
                writeLine(true);
            }
            else
            {
                pending_ += character;
            }
            return ch;
        }

        std::streamsize xsputn(const char* text, std::streamsize count) override
        {
            for (std::streamsize i = 0; i < count; ++i)
            {
                if (text[i] == '\n')
                {
                    writeLine(true);
                }
                else
                {
                    pending_ += text[i];
                }
            }
            return count;
        }

        int sync() override
        {
            // Input prompts often have no newline. Center them when cin flushes cout.
            if (!pending_.empty())
            {
                writeLine(false);
            }
            return destination_->pubsync();
        }

    private:
        void writeLine(bool addNewline)
        {
            if (!pending_.empty() && pending_[pending_.size() - 1] == '\r')
            {
                pending_.erase(pending_.size() - 1);
            }

            std::string line = prepareLine(pending_);
            const int width = visibleWidth(line);
            const int consoleWidth = getConsoleWidth();

            if (width > 0 && width < consoleWidth)
            {
                const int leftPadding = (consoleWidth - width) / 2;
                const std::string padding(static_cast<std::size_t>(leftPadding), ' ');
                destination_->sputn(padding.data(),
                                    static_cast<std::streamsize>(padding.size()));
            }

            if (!line.empty())
            {
                destination_->sputn(line.data(),
                                    static_cast<std::streamsize>(line.size()));
            }

            if (addNewline)
            {
                destination_->sputc('\n');
            }

            pending_.clear();
        }

        std::streambuf* destination_;
        std::string pending_;
    };
}

class ConsoleCentering::Impl
{
public:
    Impl()
        : originalBuffer(std::cout.rdbuf()),
          centeredBuffer(originalBuffer)
    {
        std::cout.rdbuf(&centeredBuffer);
    }

    ~Impl()
    {
        centeredBuffer.pubsync();
        std::cout.rdbuf(originalBuffer);
    }

private:
    std::streambuf* originalBuffer;
    CenteredBuffer centeredBuffer;
};

ConsoleCentering::ConsoleCentering()
    : impl_(new Impl())
{
}

ConsoleCentering::~ConsoleCentering()
{
    delete impl_;
}
