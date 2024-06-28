from PIL import Image

spritePixels = Imagem.open("path")


matrizRGB = []
rgb = []

for line in range(20):
  for col in range(20):
    coordinates = line, col
    rgbTest = spritePixels.getPixel(coordinates)
    print(rgbTest)

#https://imasters.com.br/back-end/primeiros-passos-com-pil-a-biblioteca-de-imagens-do-python
