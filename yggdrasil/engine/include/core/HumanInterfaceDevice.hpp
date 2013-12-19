#ifndef _YGG_HUMANINTERFACEDEVICE_HPP_
#define _YGG_HUMANINTERFACEDEVICE_HPP_

#include <cstdint>

namespace ygg {

/**
 * @brief The HumanInterfaceDevice class provide an abstraction to the input
 * device used by the player.
 */
class HumanInterfaceDevice {
protected:
    static const uint32_t UP_OFFSET = 0;
    static const uint32_t DOWN_OFFSET = 1;
    static const uint32_t LEFT_OFFSET = 2;
    static const uint32_t RIGHT_OFFSET = 3;
    static const uint32_t BUTTON0_OFFSET = 4;
public:
    virtual ~HumanInterfaceDevice() {
    }

    /**
     * @brief The Button enum enumarates all buttons
     * useful for the player to control his character.
     */
    enum Button {
        UP = 1 << UP_OFFSET,
        DOWN = 1 << DOWN_OFFSET,
        LEFT = 1 << LEFT_OFFSET,
        RIGHT = 1 << RIGHT_OFFSET,
        BUTTON0 = 1 << BUTTON0_OFFSET
    };

    /**
     * @brief isPressed
     * @param button The button to test
     * @return The state of the button
     */
    bool isPressed(Button button) const {
        return m_ButtonStates & button;
    }

    /**
     * @brief detectPressedEvent
     * @param button The button to test
     * @return true if the button was released at the previous frame
     * and pressed at the current frame.
     */
    bool detectPressedEvent(Button button) const {
        return m_ButtonDowns & button;
    }

    /**
     * @brief detectReleasedEvent
     * @param button The button to test
     * @return true if the button was pressed at the previous frame
     * and released at the current frame.
     */
    bool detectReleasedEvent(Button button) const {
        return m_ButtonUps & button;
    }

    /**
     * @brief update the device for a frame.
     */
    void update() {
        m_PreviousButtonStates = m_ButtonStates;
        m_ButtonStates = getButtonStates();

        // Update the button downs / ups for event detection

        // The XOR operator will put a 1 for each button that has changed its state
        uint32_t buttonChanged = m_ButtonStates ^ m_PreviousButtonStates;

        // A button is down if it is pressed and has changed its state
        m_ButtonDowns = m_ButtonStates & buttonChanged;
        // A button is up if it is not pressed and has changed its state
        m_ButtonUps = (~m_ButtonStates) & buttonChanged;
    }

protected:
    HumanInterfaceDevice():
        m_ButtonStates(0), m_PreviousButtonStates(0), m_ButtonDowns(0), m_ButtonUps(0) {
    }

    /**
     * @brief getButtonStates
     * @return The button states for the current frame.
     */
    virtual uint32_t getButtonStates() = 0;

private:
    uint32_t m_ButtonStates;
    uint32_t m_PreviousButtonStates;

    uint32_t m_ButtonDowns;
    uint32_t m_ButtonUps;
};

}

#endif // _YGG_HUMANINTERFACEDEVICE_HPP_
