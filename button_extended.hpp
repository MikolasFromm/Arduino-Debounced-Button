#include <Adafruit_MCP23X17.h>
#include "button_core.hpp"

class ButtonExtended : public ButtonBase {
public:
    /// @brief Consturctor for buttons that are connected to the MCP23X17. Requires to call Button::setup_button() before checking the button.
    /// @param p Pin number on the MCP23X17 extender
    /// @param mcp Ref. to the MCP23X17 object
    ButtonExtended(int p, Adafruit_MCP23X17& mcp) : ButtonBase(p), _mcp(mcp) {}

    /// @brief Setup the button pin on the MCP23X17
    void setup_button(){
      _mcp.pinMode(_port, INPUT_PULLUP);
    }
protected:
    bool check_current_button_state(){
        return !_mcp.digitalRead(_port); // _mpc.digitalRead == false <===> button is pressed!!!
    }
private:
    Adafruit_MCP23X17& _mcp;
};