PYTHON ?= python3
PYTHONPATH := "../shoulder"

all:
	PYTHONPATH=$(PYTHONPATH) $(PYTHON) -m scapula 
