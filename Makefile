DEVICE=Coredump
CLI=particle

all: help

login:
	${CLI} login

setup:
	${CLI} setup

list:
	${CLI} list

flash:
	${CLI} flash ${DEVICE} .

temp:
	${CLI} get ${DEVICE} temperature

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
