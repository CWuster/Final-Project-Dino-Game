import os
from pathlib import Path
from typing import Tuple
from PIL import Image


def resize_image(image_path: str, size: Tuple[int, int]) -> None:
    image = Image.open(image_path)
    image.thumbnail(size, resample=Image.NEAREST)
    new_path = Path(image_path)

    new_name = f"modified_{new_path.stem}{new_path.suffix}"
    cur_dir = Path.cwd()

    # Ensure the output directory exists
    output_dir = cur_dir / "Modified_Images"
    output_dir.mkdir(exist_ok=True)

    image.save(output_dir / new_name)


def generate_raw_data(image_path: str) -> None:
    image = Image.open(image_path)
    x, y = image.size
    pixel_image = image.load()
    new_path = Path(image_path)

    c_array = f"{new_path.stem}.h"
    c_array_path = Path.cwd() / "Generated_Code"
    c_array_path.mkdir(exist_ok=True)

    with open(c_array_path / c_array, "w") as file:
        file.write(f"#ifndef __{new_path.stem.upper()}__\n")
        file.write(f"#define __{new_path.stem.upper()}__\n\n")
        file.write(f"const unsigned char {new_path.stem}_rom[{y}][{x}] = {{\n")

        for j in range(y):
            file.write("{")
            for i in range(x):
                r, g, b, a = pixel_image[i, j]
                if a == 0:
                    file.write(f"   {0},\n")
                else:
                    color = (r * 6 // 256) * 36 + (g * 6 // 256) * 6 + (b * 6 // 256)
                    file.write(f"   {color},\n")
            file.write("},\n")

        file.write("};\n\n")
        file.write(f"#endif // __{new_path.stem.upper()}__\n")


def show_image(image_path: str) -> None:
    image = Image.open(image_path)
    image.show()


if __name__ == "__main__":
    # Ensure required folders exist
    (Path.cwd() / "Modified_Images").mkdir(exist_ok=True)
    (Path.cwd() / "Generated_Code").mkdir(exist_ok=True)

    # Input images
    dino_image_path = "./Images/dino.png"
    cactus_image_path = "./Images/cactus.png"

    # Resize and convert to C header format
    # resize_image(dino_image_path, (25, 25))
    # resize_image(cactus_image_path, (10, 10))
    generate_raw_data("./Modified_Images/modified_dino.png")
    generate_raw_data("./Modified_Images/modified_cactus.png")
