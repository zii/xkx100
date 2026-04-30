.PHONY: up down logs

DOCKER_COMPOSE = docker compose -f docker-compose.yml
UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
  DOCKER_COMPOSE += -f docker-compose.linux.yml
else
  DOCKER_COMPOSE += -f docker-compose.mac.yml
endif

up:
	$(DOCKER_COMPOSE) up -d

down:
	$(DOCKER_COMPOSE) down

logs:
	$(DOCKER_COMPOSE) logs -f

restart: down up
