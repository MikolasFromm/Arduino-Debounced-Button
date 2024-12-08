class ButtonBase{
public:
    // const expr for the delays
    static constexpr unsigned long button_delay = 80;
    static constexpr unsigned long delay_time = 300;
    static constexpr unsigned long double_click_delay = 500;
    static constexpr unsigned long short_held_delay = 2000;
    static constexpr unsigned long long_held_delay = 5000;

    /// @brief Checks if the button is currently pressed
    /// @return bool
    void check(unsigned long &time_now)
    {
        // must be stored to be able to evalute "was pressed and is not anymore" outside this function
        _previous_iteration_pressed = _prevpres;

        bool current_pressed = check_current_button_state();

        if (current_pressed == true)
        {
            if (_pressed == true)
            {
                _press_reported = true;
                _pressed = false;
            }

            if (_double_clicked == true)
            {
                _double_clicked = false;
                first_double_click = 0;
            }

            if (_short_held == true)
            {
                _short_reported = true;
                _short_held = false;
            }

            if (_long_held == true)
            {
                _long_reported = true;
                _long_held = false;
            }
        }

        if (current_pressed == true)
        {
            // new click
            if (_prevpres == false)
            {
                // If the button was pressed after the debounce-delay
                if (time_now - lastchange >= button_delay)
                {
                    if (_first_click_detected == true && (time_now - first_double_click <= double_click_delay)) // check for double-click
                    {
                        _double_clicked = true; // double click detected
                    }   
                    else
                    {
                        _first_click_detected = true; // waiting for the second click
                        first_double_click = time_now; // save the time of the first click only
                    }
                  
                    firstchange = time_now;
                    _pressed = true;
                }
            }
            else if (_short_held == false && _short_reported == false) // Spotting the short initital press
            {
                if (time_now - firstchange >= short_held_delay)
                {
                    _short_held = true;
                }
            }
            else if (_long_held == false && _long_reported == false) // Spotting the long initital press
            {
                if (time_now - firstchange >= long_held_delay)
                {
                    _long_held = true;
                }
            }
        }
        else // button is not pressed
        {
          // reset everything
          _pressed = false;
          _press_reported = false;
          _short_held = false;
          _short_reported = false;
          _long_held = false;
          _long_reported = false;
          _double_clicked = false;
          
          firstchange = time_now;
        }

        // save the time of the last change
        if (current_pressed != _prevpres)
        {
          lastchange = time_now;
        }

        // save the previous states
        _prevpres = current_pressed;
    }

    /// @brief Returns if the button is currently pressed
    /// @return bool
    bool is_pressed()
    {
        return _pressed;
    }

    /// @brief Returns if the button was pressed in any previous iteration and is still being pressed
    /// @return bool
    bool was_pressed()
    {
        return _press_reported;
    }

    /// @brief Returns if the button is held for 1sec or more
    /// @return bool
    bool is_short_held()
    {
        return _short_held;
    }

    /// @brief Returns if the button is held for 5sec or more
    /// @return
    bool is_long_held()
    {
        return _long_held;
    }

    /// @brief Returns if the button is double clicked
    /// @return bool
    bool is_double_clicked()
    {
        return _double_clicked;
    }

    /// @brief Returns if the button was just released
    /// @return bool
    bool is_released()
    {
        // was pressed in the previous interation
        // is not pressed in the current iteration
        // prevpres actually holds "current status" now
        return _previous_iteration_pressed && !_prevpres;
    }

protected:
    ButtonBase(int port) : _port(port) {}
    virtual bool check_current_button_state();
    virtual void setup_button() = 0;
    int _port;
private:
    bool _previous_iteration_pressed;
    bool _pressed;
    bool _first_click_detected;
    bool _double_clicked;
    bool _prevpres;
    bool _short_held;
    bool _long_held;

    bool _press_reported;
    bool _short_reported;
    bool _long_reported;

    unsigned long lastchange;
    unsigned long firstchange;
    unsigned long first_double_click;
};

// static_asserts for checking the delays
static_assert(ButtonBase::short_held_delay < ButtonBase::long_held_delay, "short_held_delay must be less than long_held_delay");
static_assert(ButtonBase::delay_time < ButtonBase::short_held_delay, "delay_time must be less than short_held_delay");
static_assert(ButtonBase::button_delay < 100, "Button delay is very long! If inteded, remove this static_assert.");
static_assert(ButtonBase::double_click_delay < ButtonBase::short_held_delay, "double_click_delay must be less than short_held_delay");