import shutil
import sys
from pathlib import Path

failedFiles = []
inputRaw = input("Enter input directory: ").strip('"\' ')
input_directory = Path(inputRaw).resolve()
if not input_directory.is_dir():
    sys.exit(f"Error: Input directory '{input_directory}' does not exist. \nScript terminated.")

outputRaw = input("\nEnter output directory: ").strip('"\' ')
output_directory = Path(outputRaw).resolve()
if not output_directory.is_dir():
    sys.exit(f"Error: Target directory '{output_directory}' does not exist. \nScript terminated.")

ext = input("\nEnter extension: ").lstrip(".").lower()

# Process files
for file in input_directory.iterdir():
    if file.is_file() and file.suffix.lstrip(".").lower() == ext:
        # Check free disk space before moving to prevent partial/corrupted files
        if file.stat().st_size > shutil.disk_usage(output_directory).free:
            print(f"Skipped '{file.name}': Not enough disk space on destination drive.")
            failedFiles.append((file.name, "Not enough disk space"))
            continue

        targetFile = output_directory / file.name

        # Resolve filename collisions
        counter = 1
        while targetFile.exists():
            targetFile = output_directory / f"{file.stem}_{counter}{file.suffix}"
            counter += 1

        try:
            shutil.move(str(file), str(targetFile))
            print(f"Moved: {file.name} -> {targetFile.name}")
        except OSError as reason:
            failedFiles.append((file.name, reason))
            continue

if failedFiles:
    print("\nThe following files failed:")
    for name, reason in failedFiles:
        print(f"\nFile: {name}. Reason: {reason}.")
else:
    print("\nSuccess")
