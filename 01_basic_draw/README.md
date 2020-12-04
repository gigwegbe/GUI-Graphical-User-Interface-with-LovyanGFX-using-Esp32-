# GUI Graphical User Interface with LovyanGFX using Esp32
This repository contains codebase on GUI Graphical User Interface with LovyanGFX using Esp32  TFT 3'5 with camera. 

```
Overview
```

I am using LovyanGFX made by Mr. Rabiyan in my own M5 Lite library etc., but I would like to reconfirm how to use LovyanGFX for explanation of drawing system.

- https://lang-ship.com/blog/files/LovyanGFX/

The materials summarized above are available.


```
What is LovyanGFX?

```

- https://github.com/lovyan03/LovyanGFX
A crisply tuned graphics library with some compatibility with Adafruit GFX and TFT_eSPI. Since it is registered in the library manager of Arduino IDE, you can use it easily.



```
Rough feature list of LovyanGFX

```

- LCD (board) automatic recognition
- Basic drawing function
- Sprite function
- Image processing function
- Text processing function
- Touch panel compatible
- There are various functions, but it is a typical function. After all automatic recognition of the board is convenient. It is a little troublesome when multiple LCDs are connected, but it can be used quickly as long as it is moved by a compatible board.

- This time I would like to investigate the basic drawing function. We will continue to do other functions in the future. So it suddenly becomes a basic drawing without explanation of how to use lcd.width ()!


```
About color
```




```
How to specify the color

```




```
Pointillism drawPixel (x, y, color)
```



```
Line drawing drawLine (x0, y0, x1, y1, color)
```



```
Gradient line drawing drawGradientLine (x0, y0, x1, y1, colorstart, colorend)

```


```
Draw horizontal line drawFastHLine (x, y, w, color)

```


```
Vertical line drawing drawFastVLine (x, y, h, color)

```


```
Square outline drawing drawRect (x, y, w, h, color)

```


```
Square fill drawing fillRect (x, y, w, h, color)

```


```
Clear screen fillScreen (color) / clear (color)

```


```
Fill floodFill (x, y, color) / paint (x, y, color)

```


```
Rounded square outline drawing drawRoundRect (x, y, w, h, r, color)


```


```
Rounded square fill drawing fillRoundRect (x, y, w, h, r, color)


```

```
Triangular outline drawing drawTriangle (x0, y0, x1, y1, x2, y2, color)

```


```
Triangular fill drawing fillTriangle (x0, y0, x1, y1, x2, y2, color)

```


```
Circle outline drawing drawCircle (x, y, r, color)


```



```
Circle fill drawing fillCircle (x, y, r, color)


```



```
Arc outline drawing drawCircleHelper (x, y, r, cornername, color)


```



```
Special circle fill drawing fillCircleHelper (x, y, r, corners, delta, color)


```


```
Ellipse outline drawing drawEllipse (x, y, rx, ry, color)


```


```
Ellipse fill drawing fillEllipse (x, y, rx, ry, color)


```


```
Arc outline drawing drawArc (x, y, r0, r1, angle0, angle1, color)

```

```
Arc fill drawing fillArc (x, y, r0, r1, angle0, angle1, color)


```


```

3-point Bezier curve drawing drawBezier (x0, y0, x1, y1, x2, y2, color)
```


```
4-point Bezier curve drawing drawBezier (x0, y0, x1, y1, x2, y2, x3, y3, color)
```

```
List

```


```

```
Reference
- https://lang-ship.com/blog/work/lovyangfx-1/