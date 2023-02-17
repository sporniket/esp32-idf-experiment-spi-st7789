Low level handling of the content of graphical device using a 12 bits (rgb-444) representation per pixels. 
* Given a memory space, initialize and maintain this space so that it can be transfered as is as a whole screen, or in parts
* Provide some low level drawing primitives : point, hline, vline, area
* Provide a way to set/get the color used by the drawing primitives : setActiveColor, getActiveColor
* Provide some low level block manipulation : copy (to external memory area), paste (from external memory area)