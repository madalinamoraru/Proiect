#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

// Ierarhie proprie cu baza std::exception
class GameException : public std::exception {
private:
    std::string msg;
public:
    GameException(const std::string& message) : msg("Game Error: " + message) {}
    // Suprascriem metoda what()
    const char* what() const noexcept override {
        return msg.c_str();
    }
    // Destructor virtual, obligatoriu in ierarhia de exceptii
    virtual ~GameException() noexcept {}
};

// Exceptie specifica, de exemplu pentru erori de initializare
class InitializationError : public GameException {
public:
    InitializationError(const std::string& component)
        : GameException("Failed to initialize: " + component) {}
};

#endif // GAMEEXCEPTION_H