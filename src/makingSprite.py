from PIL import Image

spritePixels = Image.open("docs\cactus-sprite-pixilart.png")
px = spritePixels.load()
px[4, 4] = (0, 0, 0)

matrizRGB = []
rgb = []

for line in range(20):
  for col in range(20):
    coordinates = line, col
    rgbValues = spritePixels.getpixel(coordinates)
    print(rgbValues[:3])
    rgb.append(rgbValues[:3])

for color in rgb:
  