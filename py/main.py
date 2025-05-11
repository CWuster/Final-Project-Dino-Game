from PIL import Image


def resize_image(image_path: str, size: tuple[int, int]) -> list[list[int]]:
    image = Image.open(image_path)
    image.thumbnail(size, resample=Image.NEAREST)
    new_image_path = image_path.strip("./")
    image.save(f"./modified_{new_image_path}")


def generate_raw_data(image_path: str) -> None:
    image = Image.open(image_path)
    x, y = image.size
    pixel_image = image.load()
    c_array = image_path.strip("./").strip(".png")
    c_array_path = c_array + ".h"

    with open(c_array_path, "w") as file:
        file.write(
            f"""typedef struct {{
            int r;
            int b;
            int g;
            }} {image_path.strip("./").strip(".png")};
        """
        )
        file.write(
            f"""
        const {c_array} image_rom []
        """
        )
    with open(c_array_path, "a") as file:
        file.write(" = {")
        file.write("\r")
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
