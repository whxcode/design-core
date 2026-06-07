#!/bin/bash
set -e
cd "$(dirname "$0")"

mkdir -p include js tmp

# 1. npm 依赖
[ ! -d node_modules ] && npm install

# 2. kiwi 运行时
[ ! -f include/kiwi.h ] && wget -q https://raw.githubusercontent.com/evanw/kiwi/master/cpp/kiwi.h -O include/kiwi.h

# 3. 合并所有 kiwi 文件
files=$(find schema -name "*.kiwi")
cat $files >tmp/schema.kiwi
sed -i '1i package schema;' tmp/schema.kiwi

# 4. 生成代码
npx kiwic --schema tmp/schema.kiwi --cpp include/schema.h
echo "✅ include/schema.h"

npx kiwic --schema tmp/schema.kiwi --ts js/schema.d.ts
echo "✅ js/schema.d.ts"

npx kiwic --schema tmp/schema.kiwi --js js/schema.js
echo "✅ js/schema.js"

# 5. 清理
rm -rf tmp
