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

    tft.fillScreen(TFT_BLACK);
    tft.drawPixel(120,120,TFT_RED); // Pointillism
    tft.drawLine(120,120,200,200, TFT_RED); // drawline 
    tft.drawGradientLine(100,100,140,140,TFT_RED, TFT_WHITE); //drawGradientLine(x0,x1,y0,y1,colorstart,colorend)
    tft.drawFastHLine(50,50,100, TFT_YELLOW); // drawFastHLine(x,y, w, color) w for width
    tft.drawFastVLine(40,40,100, TFT_WHITE);  // tft.drawFastVLine(x,y,h,color) h for height
    tft.drawRect(5,430,100,50); //tft.drawRect(x,y,h,w)
    tft.drawRect(5,300,50,100); //tft.drawRect(x,y,h,w)
    tft.fillRect(150,300,50,100,TFT_RED); //tft.drawRect(x,y,h,w,color)
    //tft.fillScreen(TFT_RED); // alternative to fillRect(0,0,_width,_height)
    //tft.clear(TFT_WHITE);
    //tft.floodFill(240,160,TFT_RED);
    //tft.paint(240,160,TFT_RED);
    tft.drawRoundRect(50,50,200,200,20); //tft.drawRoundRect(x,y,w,h,r) r rounded corners radius. 
    tft.fillRoundRect(50,50,100,100,20,TFT_WHITE); //tft.fillRoundRect(x,y,w,h,r) r rounded corners radius. 
    tft.drawCircle(200,200,20,TFT_WHITE); //tft.drawCircle(x,y,r,color)
    tft.fillCircle(230,230,20, TFT_RED); //tft.fillCircle(x,y,r,color)
    tft.drawEllipse(100,100,50,60,TFT_WHITE); // tft.drawEllipse(x,y,rx,ry,color);
    tft.fillEllipse(200,200,50,60,TFT_WHITE); // tft.fillEllipse(x,y,rx,ry,color);
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
