��    #      4  /   L        @  	  B  J  `   �  f   �  "  U  c   x  �   �  �   �  �   �	  �   "
  =   �
  )     P   C  
   �     �     �  "   �     �  &   �  %   %  $   K     p     �  3   �  6   �  	                  8  :   R     �     �     �  $   �  �  �  z  �  C  	  �   M  �     �  �  �   �  �  6  Y  �    *  3  H  h   |  A   �  �   '     �     �  e   �  \   <     �  L   �  U      9   X   ~   �   5   !  Q   G!  n   �!     "     "  $   ."  $   S"  J   x"  *   �"  &   �"     #  {   2#                                              "                !                 	                                               
                 #              

     OK, sound driver is configured.

                  ALSA  CONFIGURATOR

          will prepare the card for playing now.

     Now I'll run alsasound init script, then I'll use
     amixer to raise the default volumes.
     You can change the volume later via a mixer
     program such as alsamixer or gamix.
  
   
                   ALSA  CONFIGURATOR
                   version %s

            This script is a configurator for
    Advanced Linux Sound Architecture (ALSA) driver.


  If ALSA is already running, you should close all sound
  apps now and stop the sound driver.
  alsaconf will try to do this, but it's not 100%% sure. 
         Following card(s) are found on your system.
         Choose a soundcard to configure:
 
        No supported PnP or PCI card found.

 Would you like to probe legacy ISA sound cards/chips?

 
       The mixer is set up now for for playing.
       Shall I try to play a sound sample now?

                           NOTE:
If you have a big amplifier, lower your volumes or say no.
    Otherwise check that your speaker volume is open,
          and look if you can hear test sound.
 
   Probing legacy ISA cards might make
   your system unstable.

        Do you want to proceed?

 
 Looks like you having a Dell Dimension machine.
 On this machine, CS4232 driver should be used
 although CS46xx chip is detected.

 Shall I try to snd-cs4232 driver and probe
 the legacy ISA configuration? 
 Looks like you having a Thinkpad 600E or 770 notebook.
 On this notebook, CS4236 driver should be used
 although CS46xx chip is detected.

 Shall I try to snd-cs4236 driver and probe
 the legacy ISA configuration? 
 Shall I try all possible DMA and IRQ combinations?
 With this option, some unconventional configuration
 might be found, but it will take much longer time. 
===============================================================================

 Now ALSA is ready to use.
 For adjustment of volumes, use your favorite mixer.

 Have a lot of fun!

 
Configuring %s
Do you want to modify %s (and %s if present)? 
Configuring %s
Do you want to modify %s?            Probing legacy ISA cards

        Please select the drivers to probe:  : FOUND!! ALSA configurator Building card database.. Can't create temp file, exiting... Driver Selection ERROR: The config file doesn't exist:  Error, awk not found. Can't continue. Error, dialog or whiptail not found. No card database is found.. No legacy cards found No legacy drivers are available
   for your machine Probing legacy cards..   This may take a few minutes.. Probing:  Result Running modules-update... Running update-modules... Saving the mixer setup used for this in /etc/asound.state. Searching sound cards Soundcard Selection WARNING You must be root to use this script. Project-Id-Version: alsa-utils 1.0.9rc4a
Report-Msgid-Bugs-To: 
POT-Creation-Date: 2005-12-02 12:37+0100
PO-Revision-Date: 2005-12-02 12:39+0100
Last-Translator:  <dushistov@mail.ru>
Language-Team: Russian <ru@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
 

    Хорошо, драйвер звуковой карты настроен.

            Сейчас Настройщик ALSA

         подготовит звуковую карту для проигрования.

    Сейчас я запущу скрипт, инициализирующий alsasound, потом использую amixer
    для увеличения уровня громкости по умолчания.
    Вы можете изменить уровень громкости по умолчания позже
    с помощью таких программ как alsamixer или gamix.
   
                   Настройщик ALSA
                  версия %s

    Этот скрипт настраивает Advanced Linux Sound Architecture


   Если какие-нибудь приложения входящие в пакет ALSA уже запущены,
   вы должны закрыть все приложения работающие со звуком
   и остановаить звуковой драйвер.
   alsaconf попытается это сделать, но результат не гарантируется на все 100%%. 
        Следущее звуковая(ые) карта(ы) найдены в вашей системе.
        Выберите звуковую карту для настройки:
 
   Найдена не поддерживаемая PnP или PCI карта.

 Желаете проверить ISA звуковые карты/чипы?

 
       Микшер настроен для проигровования.
       Проиграть тестовый пример?

  Примечание:
  Если у вас большой усилитель, уменьшите уровень громкости или скажите нет.
  В противном случае проверьте, что ваши колонки включены,
  и удостоверьтесь что вы слышите тестовый пример.
 
  Поиск ISA звуковых карточек может сделать
  вашу систему нестабильной.

  Вы уверены?

 
 Похоже что вы имеем дело с машиной Dell Dimension.
 На этой машине CS4232 драйвер должен исползьовать, チップが検出されますが
 хотя чип звуковой карты определяется как CS46xx.

 Использовать snd-cs4232 и проверить ISA валидность
 этой конфигурации? 
 Кажется, мы имеем дело с ноутбуком Thinkpad 600E или 770.
 На этом ноутбуке, драйвер CS4236 должен использоваться
 хотя определяется CS46xx

 Использовать snd-cs4236 и проверить правильность ISA
 конфигурации? 
 Попробовать все доступные DMA и IRQ комбинации?
 С этой опцией, могут быть найдены некоторые нестандартные
 конфигураии, но это займет намного больше времени. 
===============================================================================

 Теперь ALSA готова к использованию.
 Для регулировки уровня громкости, используйте ваш любимый микшер.

 Оторвись по полной!

 
Настраиваем %s
Вы хотите изменить %s (и %s, если существует)? 
Настраиваем %s
Вы хотите изменить %s?     Поиск ISA звуковых карт

    Пожалуйста, выберите драйверы для проверки:  : НАШЕЛ!! Найстройщик ALSA Создаю базу данных с информацией о звуковых карточках.. Не могу создать временный файл, прекращаю работу... Выбор Драйвера ОШИБКА: Файл с настройками не существует:  Ошибка, awk не найден. Не могу продолжить работу. Ошибка, dialog или whiptail не найдены. База данных, содержащая информацию о звуковых карточках, не найдена.. Звуковых карточек не найдено Нету подходяшего драйвера для
  вашей машины Поиск звуковых карточек..  Это может занять несколько минут.. Проверка:  Результат Запускаем modules-update... Запускаем update-modules... Сохраняем настройки микшера в /etc/asound.state. Ищем звуковые карточки Выбор Звуковой карты ПРЕДУПРЕЖДЕНИЕ Вы должны быть суперпользователем, чтобы использовать этот скрипт. 