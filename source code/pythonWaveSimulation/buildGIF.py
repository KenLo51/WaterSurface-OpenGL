from PIL import Image, ImageDraw
import os

imagesFolder = "images\\"
outputGifName = "resultImg.gif"
frameSpeed = 1 # ms

# %%
def progressBar(name, value, endvalue, comment="", bar_length = 50, newLine=True):
    percent = float(value) / endvalue
    arrow = '-' * int(round(percent*bar_length) - 1) + '>'
    spaces = ' ' * (bar_length - len(arrow))
    print(f"\r{name} : [{arrow + spaces}]{round(percent*100):4}%  {comment}", end='')
    if (value == endvalue) and newLine:     
        print("\n", end='')

# %%
if __name__ == "__main__":
    images = []

    # %%
    # get file names
    files = os.listdir(imagesFolder)
    files.sort(key = lambda s: len(s))
    files = files[0::5]
    numFiles = len(files)
    for i, file in enumerate(files) :
        if(i%5 == 0)or(i+1 == numFiles):
            progressBar("Reading images", i+1, numFiles)
        # check valid format
        if(file.split('.')[-1] != "png"):
            continue
        # read image
        newImage = Image.open(os.path.join(imagesFolder,file)).convert('P') 
        images.append(newImage)

    # save gif
    print("Saving GIF")
    images[0].save(outputGifName,
                save_all=True, append_images=images[1:], optimize=False, duration=frameSpeed, loop=0)