For main:
    - gcc src/cbmp.c src/main.c -o main.exe -std=c99
    - ./main.exe samples/easy/1EASY.bmp output.bmp

For integration test:
    - gcc src/cbmp.c test/integration_test.c -o integration_test.exe -std=c99
    - ./integration_test.exe

For unit test_
    -  gcc test/unit_test.c -o unit_test.exe -std=c99
    - ./unit_test.exe

The folder 'samples' consists sample images for the 4 different levels of detection difficulty: easy, medium, hard  and impossible

The folder 'test/output' consists of all output images with the marked cells after running the integration test
