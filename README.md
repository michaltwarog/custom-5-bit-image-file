The .idk format uses color palettes of of up to 32 colors. 
It is possible to use 4 different palettes for writing and reading an image file - bit shifted, imposed, grayscale and
matched. Information about the color of a given pixel is stored as indexes in the
the specified palette. The indexes stored in succession in 5 bit blocks or on consecutive
bytes according to the RLE standard. 

Available options:
* palette variant - char (p - bit shift , b - black and white , n - imposed palate , d -
adjusted palette)
* compression - char (r - rle compression, b - no compression)

Bit shift palette - variant variable ("wariant") set to 'p'
![image](https://user-images.githubusercontent.com/24639193/213318073-8d9e0808-f8fe-423b-8b7b-a52f5123bfad.png)

Palette for nearest neighbor and dithering - variant variable ("wariant") set to 'n'.
![image](https://user-images.githubusercontent.com/24639193/213317668-c7be6c84-f639-4e13-aa17-251182748e6b.png)

Matching grayscale palette - variable variant ("wariant") set to 'b'
![image](https://user-images.githubusercontent.com/24639193/213317517-59537c3e-a1af-47d2-a45b-1a4464fc1a62.png)
