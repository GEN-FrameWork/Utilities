from pathlib import Path
from PIL import Image

root = Path(r"e:\Projects\GEN_FrameWork\Utilities\XTraceMonitorGEN\assets\graphics")
masters = root / "_masters_220"
# Match the pre-zip toolbar look: full glossy bubble in a 59x59 slot (1:1 blit).
# Wider master crop keeps icon ~50-60% of the bubble (ring+highlight visible).
out_size = 59
crop_side = 172
names = ["Stop", "Run", "Clear", "Del_Status", "DynDNS", "Save", "Filtros", "Configuracion"]
states = ["Active", "Preselected", "Pressed", "Disabled"]


def center_square(im, side):
    side = min(side, im.width, im.height)
    left = (im.width - side) // 2
    top = (im.height - side) // 2
    return (left, top, left + side, top + side)


def main():
    n = 0
    for name in names:
        for st in states:
            fn = f"{name}_{st}.png"
            src_path = masters / fn if (masters / fn).exists() else root / fn
            if not src_path.exists():
                print("MISS", fn)
                continue
            im = Image.open(src_path).convert("RGBA")
            box = center_square(im, crop_side)
            out = im.crop(box).resize((out_size, out_size), Image.Resampling.LANCZOS)
            out.save(root / fn, "PNG")
            print(f"{fn:32} crop={crop_side} -> {out.size}")
            n += 1
    print("done", n)


if __name__ == "__main__":
    main()
