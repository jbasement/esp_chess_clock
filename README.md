# esp_chess_clock
Chess clock built with esp32 d1 mini

This repo contains the wiring diagram, CAD file for the 3D printed case and the code to run on the esp32.

## Components

### Electrical

1. [ESP32 D1 Mini](https://www.az-delivery.de/en/products/esp32-d1-mini)
2. [TM1637 7-Bit display](https://www.az-delivery.de/en/products/4-digit-display)
3. [0.96 OLED display](https://www.az-delivery.de/en/products/0-96zolldisplay)
4. [KY-021 reed sensor](https://www.az-delivery.de/en/products/reed-sensor-modul)
5. [18650 battery shield](https://www.amazon.de/diymore-Battery-Micro-USB-Anschluss-Raspberry-Arduino/dp/B0822Q4VS4/ref=sr_1_6?crid=K10XXN0YTBQA&dib=eyJ2IjoiMSJ9.J-EndQcm7N3D6gvyYPXvQrBkeV-imZgiSlsFI-nJDhHIcLz3kFxL4r3rE57E1wRCW03QmH7nwVfO4Dk6v6NrV_vt05YFZKkc1wdFskPE_tU1Po6Q7cSMFX_sEwIacjYeIYGzJlw7o_H2Z7xWpy4MCYWYN5W1a2ZNIrykLu3-trAV94nS7pJ7edM3nCLX0ESjMC3VpWvqbf-Sr-O-1wwkVIfdp7Uts7sZ17HzOstx7VE.ypr87iLTZ2R-nR8sQ-8zJDfRfBbq20p0xIZTkoHp7mQ&dib_tag=se&keywords=battery+shield+18650&qid=1711971839&sprefix=battery+shiel%2Caps%2C98&sr=8-6)
6. [Tactile push button](https://www.amazon.de/RUNCCI-YUN-Drucktastenschalter-Mikroschalter-oberfl%C3%A4chenmontierte-Schaltplatte%EF%BC%8C/dp/B07WPBQXJ9/ref=sr_1_5?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2QW428MW38EQT&dib=eyJ2IjoiMSJ9.CKvSpD5R70Zy2cWw6m6TQq2_bCjSjacKJZ9jR4KdK69g-Y06AbuKfQfv1Nt5KGdJvzV4ta20x_adaZ_IhAyeiykSL7sbWQ0h-cqj32LgpW6x8mjBxgO4QMPnag3yX8hyAz-lJlP9JoSFhfb6WyGcCUut-N_WsiG2r4wTygbXthCrLKR49V33k6TFoNbGA2B1tlS3KDfGMPh1dkupuuerA9cpIQXPzD_eXNouuC6qkVLK3rE3ScgXBU5o46hqMUqsJ232Vm8JuB3k5_-mAw9_rX9VBJ43TANSWLezPpq64cQ.yyJukH0bCxscEhqo4GiVxbl1euRX_opET0x8s3_tHbs&dib_tag=se&keywords=arduino%2Bbutton&qid=1711971887&sprefix=arduiono%2Bbutton%2Caps%2C95&sr=8-5&th=1)

### Misc

1. [Tiny magnets](https://www.amazon.de/Magnetpro-Selbstklebende-Klebemagnete-Magnetisiert-Pr%C3%A4sentation/dp/B0BJQ918KX/ref=sr_1_5_pp?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=1GSN5W9B7UUWS&dib=eyJ2IjoiMSJ9.2cOQUguLfahOFVIQM8V8fxdiyYm8JDx6Du7et8__SxrMFfcAqFaga59EaoVIYsDff0ZtFTyXYdplDRwxuW0UF9ZLFqDt0MYg9uJwmin5Bx2PIo_Z7YkoHS0ONNP7VuHYp2PpygkHN6Yn-Rpie6hvaZ0sIEvYqCSpHKeC5hoyDeYdmVhAP-xWkFLeOBgRrBUSMUNC1pfjdJ83o5kiLQ1pR3l-p_YCfiG4Y6a2OR5iIztS5n5A0z5VdCuZv0JFHXAk3yOGXl-x8C012LAwAN1efsh4Jx17r-RmXm7Yzs6x578.01wgaaS0jlUSBjDc_kBEDbixVZAE_3Mhk6gNiB0LPm4&dib_tag=se&keywords=magnete&qid=1711971986&sprefix=magnete%2Caps%2C98&sr=8-5&th=1)
2. [Threaded inserts M2](https://www.amazon.de/qxayxa-Gewindeeinsatz-Einpressmutter-Gewindebuchsen-Kunststoffteile/dp/B0CJM2Z7YJ/ref=sr_1_12?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=27U5IUFEZ6S6M&dib=eyJ2IjoiMSJ9.uaJVC2zPM1zybthFuxVUeakmpGvN5nQ5uz_5TuA9vw1I-tuKI9tJCexorF_4ruXLO3ASQTpgf_EpJ-X9j2KdrIFUzfJqv63Wrbi5k2EC1mgC7Txvcw7buerFJPWxbRZQJdaMAKV-WkE8S5LKHqBEMbIbnk5yIloMO7HwBn9ASbIDEaPsFK72-y823pmY46BuUludJYPCQNn0ibpEjOpnd1mLjmRvTyHxEbOGqjHFYzNW5zBURiCJvNcXsuwlhXr4v8QVTQAyh57Y8wdWV_K6tuzEOnFWU4gtFc4xM3oBGmc.LToSlyCucixuNVnZ-JJgto-7bAWm594cnsGaPGrDX-8&dib_tag=se&keywords=gewindeeinsatz+m2&qid=1711972424&sprefix=gewindeeinsatz+m2%2Caps%2C132&sr=8-12)
3. [M2 screws](https://www.amazon.de/FandWay-Linsenkopfschrauben-Schrauben-Unterlegscheiben-Sortiment/dp/B09SH5N5T4/ref=sr_1_12?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2J01D43QSO00S&dib=eyJ2IjoiMSJ9.mVXENTTZ6ImD8Ice9r9QcvkBn-0-LpLx3v8wopXjY-1k6JuWHhwBwLCdVsTTfW_2spAphCBZovKg-utQ39Nx2piK8V-J_I3W0Clcp9KylvPbl7FskKnOhkT2BTTrz02ezanf2Jnj4SZGeZlboSFmfpiVu34aGNvKko7NM0itbBCym6lV-ls1UgRxdtiM4LlOWSHC8N__GDflgKR0Q7hUzgTi0tt6p_7Sux-HU5WwHcCAp4iMix0NPkvZvXi0iJtj00Cdn7EwHKYP66wwErSK4i6EIhEy6FpGyJDCN96MnTA.fzW77mBJeoUZDc_Dl1TNCzZQ5qboBpVudAcqOcU0hC4&dib_tag=se&keywords=m2+schrauben&qid=1711972469&sprefix=m2+schrauben%2Caps%2C93&sr=8-12)

## Assembly

Of course this is up to personal preference but this is how I assembled everything.

### Soldering

1. Parallel chain all peripheral components (Ground + VCC)
    1. Already add data to components when soldering the power lines
2. Superglue buttons to the case
3. Solder ground and data line to buttons
4. Solder every data line to the ESP32
5. Solder the power lines (ESP32, peripherals and ground for buttons) to the battery shield

### Case 

1. Superglue magnets into magnet holder
2. Add threaded inserts into holes
3. Screw in every component in place
    1. OLED display has to be superglued as case was to thin to add screws


## Known issues

1. OLED display has to be superglued into place
2. Between 5-8min fixed time the first move subtracts 2 seconds on start and not 1 as it should
