import os


def remove_backslash(input_file, output_file):
    with open(input_file, 'r') as file:
        content = file.read()
        updated_content = content.replace("\\", "")

    with open(output_file, 'w') as file:
        file.write(updated_content)

# Example usage: remove backslashes from input.txt and save to output.txt
inputPath = "/home/fudoyusea/tyy_work/data_real/lyl/java_classes.data"
resultPath = "/home/fudoyusea/tyy_work/data_real/lyl/java_classes_trans_for_ragel.data"
remove_backslash(inputPath, resultPath)