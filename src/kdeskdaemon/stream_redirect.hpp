class StreamRedirector {
public:
    explicit StreamRedirector(std::ios& stream, std::streambuf* new_buf) :
        saved_buf(stream.rdbuf()), stream_(stream)
    {
        stream_.rdbuf(new_buf);
    }

    ~StreamRedirector() {
        stream_.rdbuf(saved_buf);
    }

private:
    std::streambuf* saved_buf;
    std::ios& stream_;
};
