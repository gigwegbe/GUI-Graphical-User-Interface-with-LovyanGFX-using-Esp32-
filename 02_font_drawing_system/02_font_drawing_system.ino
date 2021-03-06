//#include <Arduino.h>
#include <Wire.h>
#include "SPI.h"
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

//Choice your touch IC
//#define NS2009_TOUCH
#define FT6236_TOUCH

#ifdef NS2009_TOUCH
#include "NS2009.h"
const int i2c_touch_addr = NS2009_ADDR;
#endif

#ifdef FT6236_TOUCH
#include "FT6236.h"
const int i2c_touch_addr = TOUCH_I2C_ADD;
#endif

//SPI control
#define SPI_ON_TFT digitalWrite(LCD_CS, LOW)
#define SPI_OFF_TFT digitalWrite(LCD_CS, HIGH)

struct LGFX_Config
{
    static constexpr spi_host_device_t spi_host = VSPI_HOST;
    static constexpr int dma_channel = 1;
    static constexpr int spi_sclk = LCD_SCK;
    static constexpr int spi_mosi = LCD_MOSI;
    static constexpr int spi_miso = LCD_MISO;
};

static lgfx::LGFX_SPI<LGFX_Config> tft;
static LGFX_Sprite sprite(&tft);
static lgfx::Panel_ILI9488 panel;

int last_pos[2] = {0, 0};
int draw_color = TFT_WHITE;

void setup()
{

    Serial.begin(115200);
    while (!Serial)
        ; // Leonardo: wait for serial monitor
    Serial.println("\n NS2009 test");

    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;

    Wire.beginTransmission(i2c_touch_addr);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(i2c_touch_addr, HEX);
        Serial.println("  !");
    }
    else if (error == 4)
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(i2c_touch_addr, HEX);
    }

    pinMode(LCD_CS, OUTPUT);
    SPI_OFF_TFT;
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    //TFT(SPI) init
    SPI_ON_TFT;

    set_tft();
    tft.begin();
    //tft.init();

    // Screen fill
    tft.fillScreen(TFT_BLACK);
    
    // Reset the cursor to the upper left
    tft.setCursor(0,0); 

    // Font specification 
    tft.setFont(& fonts :: Font0); 

    // Font color set 
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 

    tft.setTextSize(1); 
    tft.print("print"); 
    tft.println(" println"); 
    //tft.printf("printf $d \n" , 10); 
    tft.setCursor(100,100); 
    tft.setTextSize(2,4); 
    tft.println("Size2x4");

    // About the cursor(wrap true)
    // tft.setCursor(200,200);
    // tft.println(" ABCDEFGHIJKLMNOPQRSTUVWXYZ \ nabcdefghijklmnopqrstuvwxyz ");

    // // About the cursor(wrap false)
    // tft.setCursor(200,200);
    // tft.setTextWrap(false); 
    // tft.println(" ABCDEFGHIJKLMNOPQRSTUVWXYZ \ nabcdefghijklmnopqrstuvwxyz ");

    // Specify drawing range 
    tft.setCursor(64,64);
    tft.setClipRect(64,64, 128, 128); 
    tft.println("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
    tft.clearClipRect(); 
    tft.setCursor(0,0); 
    tft.println("1234567890"); 

}

void loop()
{

    int pos[2] = {0, 0};
#ifdef NS2009_TOUCH
    Serial.println(ns2009_pos(pos));
#endif
#ifdef FT6236_TOUCH
    ft6236_pos(pos);
#endif
}

void set_tft()
{
    // I will add various settings to the panel class.
    // (If you select a panel class for an LCD integrated product,
    //   The initial value is set to match the product, so no setting is required.)

    // Set the SPI clock during normal operation.
    // Esp32 SPI is only available with 80MHz broken by an integer.
    // The configurable value closest to the set value is used.
    panel.freq_write = 60000000;
    //panel.freq_write = 20000000;

    // Set the SPI clock during the single-color fill process.
    // Basically freq_write same value as the same value.
    // Setting a higher value may still work.
    panel.freq_fill = 60000000;
    //panel.freq_fill  = 27000000;

    // Set the SPI clock for reading pixel data from the LCD.
    panel.freq_read = 16000000;

    // Set spi communication mode from 0 to 3.
    panel.spi_mode = 0;

    // Set spi communication mode at the time of data reading from 0 to 3.
    panel.spi_mode_read = 0;

    // Set the number of dummy bits when reading pixels.
    //  Adjust when bit shift occurs in pixel reading.
    panel.len_dummy_read_pixel = 8;

    // Set true for panels that can read data, false if not possible.
    // If omitted, it will be true.
    panel.spi_read = true;

    // Set true for panels that are performed with the read data MOSI pin.
    // If omitted, it will be false.
    panel.spi_3wire = false;

    // Set the pin number to which the LCD CS is connected.
    // If not, omit it or set -1.
    panel.spi_cs = LCD_CS;

    // Set the pin number to which the LCD DC is connected.
    panel.spi_dc = LCD_DC;

    //  Set the pin number to which the LCD RST is connected.
    // If not, omit it or set -1.
    panel.gpio_rst = LCD_RST;

    // Set the pin number to which the LCD backlight is connected.
    //  If not, omit it or set -1.
    panel.gpio_bl = LCD_BL;

    // When using backlight, set the PWM channel number used for brightness control.
    // If you do not want to use PWM brightness control, omit it or set -1.
    panel.pwm_ch_bl = -1;

    //  Set the output level when the backlight is on low or high.
    // True when omitted. It lights up at true=HIGH / false=LOW.
    panel.backlight_level = true;

    // Set the initial value of invertDisplay.If true, it will be reversed.
    // False when omitted.If the color of the screen is inverted, change the settings.
    panel.invert = false;

    // RGB=true / BGR=false The color order of the panels is set.RGB=true / BGR=false
    // False when omitted.If red and blue are replaced, change the settings.
    panel.rgb_order = false;

    // Set the number of pixels (width and height) that the panel's memory has.
    // If the settings are not right, the coordinates when using setRotation are off.
    // (Example: ST7735 has three streets: 132x162 / 128x160 / 132x132)
    panel.memory_width = LCD_WIDTH;
    panel.memory_height = LCD_HEIGHT;

    // Set the actual number of pixels (width and height) of the panel.
    // When omitted, the default value of the panel class is used.
    panel.panel_width = LCD_WIDTH;
    panel.panel_height = LCD_HEIGHT;

    // Set the amount of offset for the panel.
    // When omitted, the default value of the panel class is used.
    panel.offset_x = 0;
    panel.offset_y = 0;

    // Set the value immediately after initializing setRotation.Set the value immediately after initializing setRotation.
    panel.rotation = 0;

    // If you want to change the direction when using setRotation offset_rotation the settings.
    // If you want the direction at setRotation(0) to be the direction at the time of 1, set 1.
    panel.offset_rotation = 0;

    // When you're done, pass the panel pointer in lgfx's setPanel function.
    tft.setPanel(&panel);
}
