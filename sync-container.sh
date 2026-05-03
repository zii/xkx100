#!/bin/bash
# 监控 kungfu/ 和 d/ 目录，文件变更时自动同步到容器 tmpfs
# 用法: ./sync-container.sh [容器名]
# 默认容器: xkx100
# 依赖: brew install fswatch

CONTAINER=${1:-xkx100}
WATCH_DIR=${2:-$(cd "$(dirname "$0")" && pwd)}

echo "监控目录: $WATCH_DIR/{kungfu,d,clone}"
echo "目标容器: $CONTAINER"
echo "按 Ctrl+C 停止"

fswatch -0 "$WATCH_DIR/kungfu" "$WATCH_DIR/d" "$WATCH_DIR/clone" | while IFS= read -r -d '' file; do
  rel="${file#$WATCH_DIR/}"
  # base64 管道方式（docker cp 在 tmpfs 上建新文件不可靠）
  base64 "$file" | docker exec -i "$CONTAINER" sh -c 'base64 -d > "/mudlib/'"$rel"'"'
  echo "[$(date '+%H:%M:%S')] synced: $rel"
done
