#include <UnitTest++/UnitTest++.h>
#include <iostream>

int main(int, const char*[]) {
    std::cout << "ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ" << std::endl;

    int failureCount = UnitTest::RunAllTests();

    if (failureCount == 0) {
        std::cout << ">>> ВСЕ МОДУЛЬНЫЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ! <<<" << std::endl;
    } else {
        std::cout << ">>> ОБНАРУЖЕНЫ ОШИБКИ! Не пройдено тестов: " << failureCount << " <<<" << std::endl;
    }

    return failureCount;
}

