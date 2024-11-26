#!/bin/zsh

UE_EDITOR_PATH="/Volumes/T9/UE5_version/UE_5.4/Engine/Binaries/Mac/UnrealEditor.app"

PROJECT_PATH="/Volumes/T9/UnrealEngineProject/SwordOrShield/SwordAndShield.uproject"

if [ -d "$UE_EDITOR_PATH" ]; then
    echo "Mở UnrealEditor với project: $PROJECT_PATH"
    open -a "$UE_EDITOR_PATH" "$PROJECT_PATH" 
else
    echo "Không tìm thấy UnrealEditor.app tại đường dẫn: $UE_EDITOR_PATH"
    exit 1
fi
