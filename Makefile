DEVICE=Coredump

all: help

login:
	particle login

setup:
	particle setup

list:
	particle list

flash:
	particle flash ${DEVICE} .

temp:
	particle get ${DEVICE} temperature

help:
	@echo 'Supported make targets: login, setup, list, flash, temp, help'
	@echo ''
	@echo ' login -- Log in to your Particle account.'
	@echo ' setup -- Set up a new Particle device.'
	@echo ' list  -- List all Particle devices.'
	@echo ' flash -- Flash the firmware to your $$DEVICE.'
	@echo ' temp  -- Show the temperature of your $$DEVICE.'
	@echo ' help  -- Show this help.'
	@echo ''

PHONY: list flash
