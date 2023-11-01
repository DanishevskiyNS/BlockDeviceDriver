# Улучшение яркости изображения
## Описание задачи
* Основная цель задачи - улучшить яркость изображения исходных картинок
* Язык программирования Python
* Ограничений по использованию библиотек и сторонних функций нет

## Датасет
Тренировочный датасет включает 745 картинок  
[Ссылка на датасет](https://drive.google.com/file/d/1ThoPb1flnfXDpRIytgBd7_e9Kv_lPnbo/view) 
Пример загрузки датасета в PyTorch приведён в файле dataset.py

## Метрики
Для оценки качества используются следующие метрики:
* [PSNR](https://ru.wikipedia.org/wiki/Пиковое_отношение_сигнала_к_шуму) - Пиковое отношение сигнала к шуму;
* [SSIM](https://ru.wikipedia.org/wiki/SSIM) - Индекс структурного сходства;
* [LPIPS](https://github.com/richzhang/PerceptualSimilarity#c-about-the-metric) - Learned Perceptual Image Patch Similarity
Пример расчёт метрик представлен в файле evaluation.py.

## Результаты
На обработку одного изображения у нашего решения уходит 1,5 (мс).
Таблица содержит результаты вычисления метрик при разном размере батча.
|batch size|PSNR(🠕)|SSIM(🠕)|LPIPS(🠗)|
:---:|:---:|:---:|:---:
1|13.07|0.63|0.24
2|18.25|0.70|0.24
4|23.97|0.77|0.24
8|29.71|0.82|0.24
