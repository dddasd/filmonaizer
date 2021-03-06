Фильмонайзер / Filmonizer
=========================

- **Текущая версия:** 0.3.25
- **Домашная страница:** http://wiki.soulruins.info/filmonaizer/start

Описание:
---------
Фильмонайзер - универсальный кроссплатформенный инструмент, призванный помочь вам в нелёгком деле визуального оформления сообщений о фильмах. Будь то сообщения на форуме или трекере, в твиттере или аське, Фильмонайзер оформит текст сообщения именно так, как вам необходимо. На основе заданного вами шаблона, он создаст готовый к вставке текст с полной информацией о фильме и скачает обложку. Ещё один способ использования Фильмонайзера - создание `*.html/*.comment/*.ion/*.nfo/*.txt` (на самом деле любых) файлов с информацией, которые смогут послужить дополнительными помощниками при скачивании фильмов (например с фтп). В последствии, подобные файлы так же могут быть импортированы разными Медиа-центрами (если это позволяет встроенный функционал).

Особенности:
------------
- кроссплатформенность (работает в **Windows** и **Linux** системах)
- работает с любыми видами тегов (`bbcode`, `xhtml` и т.п.)
- экспорт в *twitter* и *friendfeed* (и другие)
- экспорт данных в любой формат (`txt`, `nfo`, `html` и т.п.)
- ищет и скачивает обложки
- гибкая настраиваемая система шаблонов
- понимает как русские, так и оригинальные названия фильмов
- импорт тех.данных о фильме
- встроенный редактор шаблонов с подсветкой синтаксиса
- поддержка прокси
- мультиязычность интерфейса

Авторы:
-------
- Philip (aka soulruins) - Идея и дизайн. e-mail: soulruins@gmail.com
- dddasd - Разработка. e-mail: ddd.asd@gmail.com

Благодарности:
--------------
- http://www.pinvoke.com - за иконки
- http://mediainfo.sourceforge.net - за библиотеку mediainfo
- http://kinopoisk.ru - за информацию о фильмах
- http://themoviedb.org - за информацию о фильмах

Установка:
----------
UBUNTU (проверено на версии 15.04):
```sh
$sudo apt-get install libqt5-dev qt5-default
$qmake filmonaizer.pro
$make
```
