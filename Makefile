# ANSI color codes
YELLOW := \033[1;33m
GREEN := \033[0;32m
RED := \033[0;31m
RESET := \033[0m

# Enter shell
shell:
	@echo "$(YELLOW)Entering shell...$(RESET)"
	@if [ ! -d ".venv" ]; then \
		echo "$(YELLOW)Creating virtual environment...$(RESET)"; \
		pipenv install; \
	fi
	@pipenv shell

# Run GUI code
gui:
	@echo "$(YELLOW)Running GUI code...$(RESET)"
	@if [ -z "$$VIRTUAL_ENV" ]; then \
		echo "$(RED)Not in a virtual environment. Please run 'make shell' first.$(RESET)"; \
		exit 1; \
	fi
	@echo "$(GREEN)Starting GUI...$(RESET)"
	@python src/GUI/main.py

