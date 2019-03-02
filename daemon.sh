#!/usr/bin/env bash
sudo cp garage.service /etc/systemd/system
sudo systemctl start garage
sudo systemctl enable garage
sudo systemctl daemon-reload
