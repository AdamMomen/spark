#!/bin/bash
echo "🔄 Switching back to Clangd..."
mv .clangd.backup .clangd 2>/dev/null || echo "No .clangd backup found"
echo "✅ Clangd configuration restored"
echo "Please restart your editor to apply changes"
