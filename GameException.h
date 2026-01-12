#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::exception {
private:
    std::string msg;
public:
    GameException(const std::string& message) : msg("Game Error: " + message) {}
    const char* what() const noexcept override {
        return msg.c_str();
    }
    virtual ~GameException() noexcept {}
};


class InitializationError : public GameException {
public:
    InitializationError(const std::string& component)
        : GameException("Failed to initialize: " + component) {}
};

#endif // GAMEEXCEPTION_H