#include "button_core.hpp"

class ButtonClassic : public ButtonBase {
public:
    /// @brief Consturctor for buttons that are connected straight to the board
    /// @param p Pin number on the board
    ButtonClassic(int p) : ButtonBase(p) {}

    /// @brief Setup the button pin with INPUT_PULLUP
    void setup_button(){
        pinMode(_port, INPUT_PULLUP);
    }
protected:
    bool check_current_button_state(){
        return !digitalRead(_port); /// digitalRead == false <===> button is pressed!!!
    }
};