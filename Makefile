DEVICE=Coredump
CLI=particle

all: help

login:
	${CLI} login

setup:
	${CLI} setup

list:
	${CLI} list

flash-ota:
	${CLI} flash ${DEVICE} .

flash-usb:
	${CLI} flash --usb ${DEVICE} .

temp:
	${CLI} get ${DEVICE} temperature

help:
	@echo 'Supported make targets: login, setup, list, flash-ota, flash-usb, temp, help'
	@echo ''
	@echo ' login     -- Log in to your Particle account.'
	@echo ' setup     -- Set up a new Particle device.'
	@echo ' list      -- List all Particle devices.'
	@echo ' flash-ota -- Flash the firmware to your $$DEVICE over the air.'
	@echo ' flash-usb -- Flash the firmware to your $$DEVICE over USB.'
	@echo ' temp      -- Show the temperature of your $$DEVICE.'
	@echo ' help      -- Show this help.'
	@echo ''

PHONY: list flash
