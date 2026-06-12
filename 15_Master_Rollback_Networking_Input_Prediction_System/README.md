# 15 Master - Rollback Networking, Input Prediction System

This folder teaches the basic idea behind rollback networking.

## What You Learn

- Predict local movement immediately from input.
- Store past predicted states.
- Receive an authoritative server state for an older frame.
- Roll back to the server state and replay later inputs.
- Compare predicted state before and after correction.

## Why It Matters

Fast action games cannot wait for the server before moving the local player. Rollback lets the game feel responsive while still correcting mistakes when authoritative data arrives. This idea is used in fighting games, action games, rollback netcode, replay tools, and automated simulation tests.
