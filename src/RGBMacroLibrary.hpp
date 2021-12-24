// Define our shared functions
void SetupButton(uint8_t ButtonNum, uint8_t r, uint8_t g, uint8_t b, int KeyCode, int ModifierKeys, int KeyboardType);
void RemoveButtonSetup(int ButtonNum);
void InitializeDevice(void);
void MacropadLoop(bool UseBlinking, int DimLedDuration);
