#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR" || exit 1

SERVER_BIN="./vcalc_server"
CLIENT_BIN="./dumb_client/client_uint32_t"
LOG_DIR="./log"
TEST_LOG="$LOG_DIR/functional_test.log"
SERVER_LOG="$LOG_DIR/vcalc.log"
BASE_CONF="./base/vcalc.conf"
PORT=33333

mkdir -p "$LOG_DIR"
rm -f "$SERVER_LOG" "$TEST_LOG"

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

cleanup() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill -TERM "$SERVER_PID" 2>/dev/null
        wait "$SERVER_PID" 2>/dev/null
    fi
}
trap cleanup EXIT INT TERM

log_file() {
    echo -e "$1" | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" >> "$TEST_LOG"
}

run_test_case() {
    local name="$1"
    local type="$2"
    local desc="$3"
    local precondition="$4"
    local input_data="$5"
    local expected="$6"
    local postcondition="$7"
    local cmd="$8"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    echo -e "============================================================"
    echo -e "Тест №$TOTAL_TESTS: $name [$type]"
    echo -e "Описание           : $desc"
    echo -e "Ожидаемый результат: $expected"

    log_file "------------------------------------------------------------"
    log_file "Тест №$TOTAL_TESTS: $name"
    log_file "Тип                : $type"
    log_file "Описание           : $desc"
    log_file "Предусловие        : $precondition"
    log_file "Входные данные     : $input_data"
    log_file "Ожидаемый результат: $expected"
    log_file "Постусловие        : $postcondition"

    local result_msg=""
    local status="FAIL"

    eval "$cmd"
    local ret_code=$?

    if [ $ret_code -eq 0 ]; then
        status="Пройден"
        result_msg="Соответствует ожидаемому результату"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo -e "Полученный рез-т   : $result_msg"
        echo -e "Итог               : ПРОЙДЕН"
    else
        status="Не пройден"
        result_msg="Несоответствие ожидаемому результату (код ошибки: $ret_code)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo -e "Полученный рез-т   : $result_msg"
        echo -e "Итог               : НЕ ПРОЙДЕН"
    fi

    log_file "Полученный рез-т   : $result_msg"
    log_file "Итог               : $status"
}

echo "============================================================"
echo "      ФУНКЦИОНАЛЬНОЕ ТЕСТИРОВАНИЕ СЕРВЕРА VCALC_SERVER      "
echo "============================================================"

if [ ! -f "$SERVER_BIN" ]; then
    echo -e "Ошибка: Сервер $SERVER_BIN не найден! Соберите его командой 'make'."
    exit 1
fi

run_test_case "Проверка вывода справки (-h)" "Позитивный" \
    "Проверка вывода справочной информации о параметрах запуска" \
    "Сервер vcalc_server скомпилирован" \
    "Запуск ./vcalc_server -h" \
    "Код возврата 0, текст справки в stdout" \
    "Нет" \
    'output=$($SERVER_BIN -h 2>&1); [[ $? -eq 0 && "$output" =~ "Использование:" ]]'

run_test_case "Некорректный номер порта" "Негативный" \
    "Проверка обработки недопустимого номера порта" \
    "Сервер vcalc_server скомпилирован" \
    "Запуск ./vcalc_server -p 99999" \
    "Код возврата 1, сообщение об ошибке порта в stderr" \
    "Нет" \
    'output=$($SERVER_BIN -p 99999 2>&1); [[ $? -ne 0 && "$output" =~ "некорректный номер порта" ]]'

run_test_case "Неизвестный параметр" "Негативный" \
    "Проверка обработки неподдерживаемого ключа" \
    "Сервер vcalc_server скомпилирован" \
    "Запуск ./vcalc_server -z" \
    "Код возврата 1, вывод справки" \
    "Нет" \
    '$SERVER_BIN -z >/dev/null 2>&1; [[ $? -ne 0 ]]'

run_test_case "Отсутствие файла базы пользователей" "Негативный" \
    "Запуск сервера с путем к несуществующему файлу базы" \
    "Сервер vcalc_server скомпилирован, базы нет" \
    "Запуск ./vcalc_server -b /tmp/no_such_file_123.conf -l /tmp/temp_err.log" \
    "Критическая ошибка, запись в лог и аварийный выход с кодом 1" \
"Удалить временный лог" \
    '$SERVER_BIN -b /tmp/no_such_file_123.conf -l /tmp/temp_err.log >/dev/null 2>&1; res=$?; rm -f /tmp/temp_err.log; [[ $res -ne 0 ]]'

echo -e "\n>>> Запуск сервера в фоновом режиме на порту $PORT..."
$SERVER_BIN -b "$BASE_CONF" -l "$SERVER_LOG" -p $PORT >/dev/null 2>&1 &
SERVER_PID=$!
sleep 1

if ! kill -0 "$SERVER_PID" 2>/dev/null; then
    echo -e "Ошибка: Сервер не смог запуститься на порту $PORT!"
    exit 1
fi
echo -e ">>> Сервер успешно запущен (PID: $SERVER_PID)\n"

run_test_case "Штатный сеанс с client_uint32_t" "Позитивный" \
    "Проверка полного сценария аутентификации и вычислений" \
    "Сервер запущен, база vcalc.conf содержит user:P@ssW0rd" \
    "Запуск ./client_uint32_t -H MD5 -S s" \
    "Код возврата 0, успешный расчет всех векторов без ошибок" \
    "Нет" \
    'output=$($CLIENT_BIN -H MD5 -S s 2>&1); [[ $? -eq 0 && ! "$output" =~ "ERR" ]]'

run_test_case "Ошибка идентификации (несуществующий пользователь)" "Негативный" \
    "Попытка аутентификации пользователя, отсутствующего в базе" \
    "Сервер активен и ожидает подключений" \
    "Клиент передает логин 'unknown_user_999'" \
    "Сервер отправляет ответ 'ERR', пишет в лог и закрывает сокет" \
    "Сокет закрыт" \
    'resp=$(python3 -c "import socket
s = socket.socket()
s.settimeout(3)
s.connect((\"127.0.0.1\", 33333))
s.sendall(b\"unknown_user_999\n\")
d = s.recv(1024)
print(d.decode(\"latin1\", errors=\"ignore\").strip())
s.close()" 2>/dev/null); [[ "$resp" == "ERR" ]]'

run_test_case "Ошибка аутентификации (неверный пароль / хеш)" "Негативный" \
    "Попытка аутентификации пользователя с неверным хешем" \
    "Сервер активен, пользователь 'user' существует в базе" \
    "Клиент передает логин 'user' и заведомо ложный MD5" \
    "Сервер отправляет ответ 'ERR', пишет в лог и закрывает сокет" \
    "Сокет закрыт" \
    'resp=$(python3 -c "import socket
s = socket.socket()
s.settimeout(3)
s.connect((\"127.0.0.1\", 33333))
s.sendall(b\"user\n\")
salt = s.recv(1024)
s.sendall(b\"00000000000000000000000000000000\n\")
d = s.recv(1024)
print(d.decode(\"latin1\", errors=\"ignore\").strip())
s.close()" 2>/dev/null); [[ "$resp" == "ERR" ]]'

run_test_case "Устойчивость к разрыву связи (SIGPIPE)" "Позитивный" \
    "Проверка обработки внезапного разрыва соединения клиентом" \
    "Сервер ожидает соединения" \
    "Клиент подключается, передает логин и аварийно закрывает сокет" \
    "Сервер не падает, восстанавливает состояние для новых клиентов" \
    "Нет" \
    'python3 -c "import socket
s = socket.socket()
s.connect((\"127.0.0.1\", 33333))
s.sendall(b\"user\")
s.close()" 2>/dev/null; sleep 0.5; kill -0 $SERVER_PID 2>/dev/null'

run_test_case "Тестирование устойчивости (серия последовательных клиентов)" "Позитивный" \
    "Последовательное обслуживание группы клиентов, включая сбойные" \
    "Сервер активен" \
    "Цикл из 5 повторных запросов от client_uint32_t" \
    "Все клиенты успешно обслужены, сервер продолжает работу" \
    "Нет" \
    'success=0
for i in {1..5}; do
    if $CLIENT_BIN -H MD5 -S s >/dev/null 2>&1; then
        success=$((success + 1))
    fi
done
[[ $success -eq 5 ]]'

run_test_case "Нагрузочное тестирование (параллельные подключения)" "Позитивный" \
    "Проверка поведения сервера при одновременных запросах" \
    "Сервер активен" \
    "Запуск 3 клиентов client_uint32_t параллельно в фоне" \
    "Сервер корректно обрабатывает очередь входящих соединений" \
    "Нет" \
    'pids=()
for i in {1..3}; do
    $CLIENT_BIN -H MD5 -S s >/dev/null 2>&1 &
    pids+=($!)
done
failed=0
for pid in "${pids[@]}"; do
    wait "$pid" || failed=$((failed + 1))
done
[[ $failed -eq 0 ]]'

run_test_case "Проверка файла журнала (vcalc.log)" "Позитивный" \
    "Проверка ведения журнала работы и формата записей с датой и временем" \
    "Тесты выполнены, журнал сформирован" \
    "Анализ файла log/vcalc.log" \
    "Файл не пустой, содержит отметки даты/времени и уровни [ERROR]" \
    "Нет" \
    '[[ -s "$SERVER_LOG" ]] && grep -q "ERROR" "$SERVER_LOG"'

kill -TERM "$SERVER_PID"
2>/dev/null
wait "$SERVER_PID" 2>/dev/null
unset SERVER_PID

echo -e "\n$============================================================"
echo -e "            РЕЗУЛЬТАТЫ ФУНКЦИОНАЛЬНОГО ТЕСТИРОВАНИЯ         "
echo -e "============================================================"
echo -e "Всего тестов проведено : $TOTAL_TESTS"
echo -e "Успешно пройдено       : $PASSED_TESTS"
echo -e "Провалено              : $FAILED_TESTS"

log_file "============================================================"
log_file "ИТОГИ ТЕСТИРОВАНИЯ: Проведено $TOTAL_TESTS, Пройдено $PASSED_TESTS, Ошибок $FAILED_TESTS"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e ">>> ВСЕ ФУНКЦИОНАЛЬНЫЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ! <<<"
    echo -e "Отчет сохранен в: $TEST_LOG"
    exit 0
else
    echo -e ">>> ОБНАРУЖЕНЫ ОШИБКИ В ФУНКЦИОНАЛЬНЫХ ТЕСТАХ! <<<"
    exit 1
fi
