import os
from pathlib import Path
from PIL import Image


def resize_image(image_path: str, size: tuple[int, int]) -> None:
    image = Image.open(image_path)
    image.thumbnail(size, resample=Image.NEAREST)
    new_path = Path(image_path)

    new_name = f"modified_{new_path.stem}{new_path.suffix}"
    cur_dir = Path.cwd()

    image.save(cur_dir / new_name)


def generate_raw_data(image_path: str) -> None:
    image = Image.open(image_path)
    x, y = image.size
    pixel_image = image.load()

    new_path = Path(image_path)

    c_array = f"{new_path.stem}" + ".h"

    c_array_path = Path.cwd()

    with open(c_array_path / c_array, "w") as file:
        file.write(f"#ifndef __{new_path.stem.upper()}__")
        file.write(os.linesep)
        file.write(f"#define __{new_path.stem.upper()}__")
        file.write(os.linesep)
        file.write(
            f"""typedef struct {{
            int r;
            int b;
            int g;
            }} {new_path.stem};
        """
        )
        file.write(
            f"""
        const {new_path.stem} image_rom []
        """
        )
        file.write(" = {")
        file.write(os.linesep)
        for j in range(y):
            for i in range(
                x
            ):  # we leave out the alpha so we take the first three elements
                file.write(
                    f"""
                           {{{str(pixel_image[j,i][0:3]).strip("()")}}}, 
                """
                )
        file.write("  };")
        file.write(os.linesep)
        file.write(f"#endif //__{c_array.upper()}__")


def show_image(image_path: str) -> None:
    image = Image.open(image_path)
    image.show()


if __name__ == "__main__":
    dino_image_path = "./dino.png"
    cactus_image_path = "./cactus.png"

    resize_image(dino_image_path, (50, 50))
    resize_image(cactus_image_path, (10, 10))
    generate_raw_data("./modified_dino.png")
    generate_raw_data("./modified_cactus.png")
