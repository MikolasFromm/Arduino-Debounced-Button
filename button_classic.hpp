#include "button_core.hpp"

class ButtonClassic : public ButtonBase {
public:
    /// @brief Consturctor for buttons that are connected straight to the board
    /// @param p Pin number on the board
    ButtonClassic(int p) : ButtonBase(p) {}
protected:
    bool check_current_button_state(){
        return !digitalRead(_port); /// digitalRead == false <===> button is pressed!!!
    }
};