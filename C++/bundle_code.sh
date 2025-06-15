OUTPUT_FILE="sample_project.txt"

> "$OUTPUT_FILE"

for file in $(find include -type f -name "*.hpp" | sort); do
    echo -e "\n// Содержимое файла $file\n" >> "$OUTPUT_FILE"
    cat "$file" >> "$OUTPUT_FILE"
    echo -e "\n" >> "$OUTPUT_FILE"
done

for file in $(find src -type f -name "*.cpp" | sort); do
    echo -e "\n// Содержимое файла $file\n" >> "$OUTPUT_FILE"
    cat "$file" >> "$OUTPUT_FILE"
    echo -e "\n" >> "$OUTPUT_FILE"
done

echo "Код собран в файл $OUTPUT_FILE"
