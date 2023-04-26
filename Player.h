#include <string>
#include <chrono>

class Player {
public:
    Player(const std::string& name) : name_(name) {}

    const std::string& getName() const {
        return name_;
    }

    void startTimer() {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    void winGame() {
        win_time_ = std::chrono::high_resolution_clock::now();
    }

    double getTimeElapsed() const {
        if (win_time_.time_since_epoch().count() == 0) {
            return -1.0; // Player did not win
        } else {
            std::chrono::duration<double> elapsed = win_time_ - start_time_;
            return elapsed.count();
        }
    }

private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    std::chrono::time_point<std::chrono::high_resolution_clock> win_time_;
};
