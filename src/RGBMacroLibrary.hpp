// Define our shared functions
void SetupButton(uint8_t ButtonNum, uint8_t r, uint8_t g, uint8_t b, uint8_t KeyCode, uint8_t ModifierKeys, uint8_t KeyboardType);
void RemoveButtonSetup(int ButtonNum);
void InitializeDevice(void);
void MacropadLoop(bool UseBlinking, int DimLedDuration);
