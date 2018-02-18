# Fluff64
Fluff64 (working title) will eventually be a complete toolset for developing stuff for old 8-bit computers.

For now, Fluff64 is a photoshop-like image editor that lets you import, create, modify and export old-skool commodore 64 image formats.

Currently supports:
  - Import of png/jpg images with brightness, saturation, blur and gamma corrections. In addition, color picking from your current palette.
  - C64 "fake" mode, basically a regular 320x200 image with custom palettes (C64/CGA etc)
  - C64 Multicolor mode (160x200x2 bit + 16 color palette). Experience the joy of 2-bit coloring! Images in this mode can be exported directly to a C64 assembly program that will run directly in most C64 assemblers. Code for displaying the image is also included.
  - C64 HiRes mode (320x200x1 bit + 16 color palette). Same as above, but more awesome resolution but fewer colors
  - CGA native (not implemented yet)
  - Edit multiple images at the same time, and copy data between them
  - Undo / redo (redo not yet implemented)
  - Convenient 40x25 C64 grid
  
  - Current tools include:
    - Regular brush
    - Circles
    - Random spray
    - Dithered brush (0/1 pixel shift)
    - Lines, angular lines
    - Fill
    - Copy / paste
    
   - Current convenient stuff:
    - Undo
    - Panning
    - Zoom anywhere
  
  
  To do:
    - Export to a common Fluff64 format
    - Export HiRes assembler
    - C64 character mode with automatic assembly of chars
    - Export above mode with side scroller
    - Music overlay
    - Demo effects templates
    - Sprite editor and exporter
    - Level designer
    
