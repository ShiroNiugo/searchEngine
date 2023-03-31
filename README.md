# Search engine

## Описание проекта:
>Итоговый проект на разработку "Локального поискового движка по файлам".\
>Поисковой движок представляет собой консольное приложение осуществляющее поиск и имеющий 
> возможность настройки через файлы формата JSON.

## Стек используемых технологий:
> * [IDE Clion](https://www.jetbrains.com/clion/)
> * MinGW
> * СMake
> * Git
> * JSON
> * [Nlohmann JSON](https://github.com/nlohmann/json)

## Инструкция:
> 1. Перед запуском приложения в конфигурационном файле (config.json) нужно задать названия файлов,
> по которым будет осуществляться поиск. Если данного файла не будет или он находиться не в директории
> с проектом, то при запуске приложения будет выведено ошибка "config file is missing". При этом 
> каждый документ содержит не более 1000 слов с максимальной длиной каждого в 100 символов. Слова 
> состоят из строчных латинских букв и разделены одним или несколькими пробелами.\
> Пример описания файла config.json:
>```json 
>{
>  "config": {
>    "name": "SkillboxSearchEngine",
>    "version": "0.1",
>    "max_responses": 5
>  },
>  "files": [
>    "../resources/file001.txt",
>    "../resources/file002.txt",
>    "../resources/file003.txt",
>    "../resources/file004.txt"
>  ]
>}
>```
> 2. Дальше пользователь задает запрос через JSON-файл (requests.json). Поле запросов может содержать
> не более 1000 запросов и каждый из них от 1 до 10 слов. Все слова состоят из строчных латинских букв\
> Пример описания файла requests.json:
>```json
>{
>  "requests": [
>    "some words..",
>    "some words..",
>    "some words..",
>    "some words.."
>  ]
>}
>```
> 3. Произвести запуск приложения.
> 4. Приложение самостоятельно считает и обойдет все файлы для дальнейшего индексирования слов 
> в файлах, чтобы наиболее быстро получить результат.
> 5. В конце программа формирует файл answers.json, в который записывает результаты поиска.