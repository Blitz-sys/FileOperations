import sys
from pathlib import Path

directory_raw = input("\nEnter directory: ").strip('"\' ')
directory = Path(directory_raw).resolve()
if not directory.is_dir():
    sys.exit(f"\nError: Directory '{directory}' does not exist. \nScript terminated.")

current_ext = input("\nEnter current extension: ").lstrip(".").lower()
target_ext = "." + input("\nEnter new extension: ").lstrip(".").lower()

for file in directory.iterdir():
    if file.is_file() and file.suffix.lstrip(".").lower() == current_ext:
        stem = file.stem
        new_file = directory / (stem + target_ext)

        counter = 1
        while new_file.exists():
            new_file = directory / f"{stem}_{counter}{target_ext}"
            counter += 1

        file.rename(new_file)
        print(f"Renamed: {file.name} -> {new_file.name}")

print("\nSuccess")
