from PIL import Image

spritePixels = Image.open("docs/frog.png")

matrizRGB = []
rgb = []

for line in range(20):
  for col in range(20):
    coordinates = col, line
    rgbValues = spritePixels.getpixel(coordinates)
    #print(rgbValues[:3])
    rgb.append(rgbValues[:3])

transparentColor = [6, 7, 7]

for color in rgb:
  if color == (0, 0, 0):
    matrizRGB.append(transparentColor)
  else:
    newColor = []
    for number in color:
      convertedRGB = int(number/32)
      newColor.append(convertedRGB)
    matrizRGB.append(newColor)

print(matrizRGB)
