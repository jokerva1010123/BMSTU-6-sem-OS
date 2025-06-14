Системный вызов open() открывает файл, указанный в pathname. Если
указанный файл не существует, он может (необязательно) (если указан флаг
O_CREATE) быть создан open().

Возвращаемое значение open() — дескриптор файла, неотрицательное
целое число, которое используется в последующих системных вызовах для
работы с файлом.
Параметр flags - это флаги, которые собираются с помощью побитовой
операции ИЛИ из таких значений, как:
O_EXEC — открыть только для выполнения (результат не определен,
при открытии директории).
O_RDONLY — открыть только на чтение.
O_RDWR — открыть на чтение и запись.
O_SEARCH — открыть директорию только для поиска (результат не
определен, при использовании с файлами, не являющимися директорией).
O_WRONLY — открыть только на запись.
O_APPEND — файл открывается в режиме добавления, перед каждой
операцией записи файловый указатель будет устанавливаться в конец файла.
O_CLOEXEC — устанавливает флаг close-on-exec для нового файлового дескриптора, указание этого флага позволяет программе избегать дополнительных операций fcntl F_SETFD для установки флага FD_CLOEXEC.
O_CREAT — если файл не существует, то он будет создан.
O_DIRECTORY — если файл не является каталогом, то open вернёт
ошибку.
O_DSYNC — файл открывается в режиме синхронного ввода-вывода
(все операции записи для соответствующего дескриптора файла блокируют
вызывающий процесс до тех пор, пока данные не будут физически записаны).
O_EXCL — если используется совместно с O_CREAT, то при наличии
уже созданного файла вызов завершится ошибкой.
O_NOCTTY — если файл указывает на терминальное устройство, то
оно не станет терминалом управления процесса, даже при его отсутствии.
O_NOFOLLOW — если файл является символической ссылкой, то open
вернёт ошибку.
O_NONBLOCK — файл открывается, по возможности, в режиме nonblocking, то есть никакие последующие операции над дескриптором файла не
заставляют в дальнейшем вызывающий процесс ждать.
O_RSYNC — операции записи должны выполняться на том же уровне,
что и O_SYNC.
O_SYNC — файл открывается в режиме синхронного ввода-вывода
(все операции записи для соответствующего дескриптора файла блокируют
вызывающий процесс до тех пор, пока данные не будут физически записаны).
O_TRUNC — если файл уже существует, он является обычным файлом
и заданный режим позволяет записывать в этот файл, то его длина будет
урезана до нуля.
O_LARGEFILE — позволяет открывать файлы, размер которых не может быть представлен типом off_t (long). Для установки должен быть указан
макрос _LARGEFILE64_SOURCE
O_TMPFILE — при наличии данного флага создаётся неименованный
временный файл.
O_PATH — получить файловый дескриптор, который можно использовать для двух целей: для указания положения в дереве файловой системы и для выполнения операций, работающих исключительно на уровне файловых дескрипторов. Если O_PATH указан, то биты флагов, отличные от
O_CLOEXEC, O_DIRECTORY и O_NOFOLLOW, игнорируются.
Третий параметр mode всегда должен быть указан при использовании
O_CREAT; во всех остальных случаях этот параметр игнорируется.

Функция build_open_flags используется для построения флагов доступа
Функция getname_flags используется для определения флагов, которые будут переданы в системный вызов "open" при открытии файла. 
Функция alloc_fd() используется в контексте системного вызова open() для выделения дескриптора файлового дескриптора (file descriptor) из таблицы открытых файлов процесса.
Функция path_openat() используется для открытия файла в рамках указанного каталога файловой системы
Функция open_lat_lookups() отвечает за поиск файла в файловой системе при выполнении системного вызова openat().

dentry - Это структура, описывающая элемент пути 
inode - Структура, описывающая файл на диске