## How to use
- ����freecars�ļ��е�����Ŀ¼�����ͷ�ļ�·��
- �������½�freecarsĿ¼�����[FreeCars_isr.c](https://github.com/while0l1/stm32f4_modules/blob/master/%E6%A8%A1%E5%9D%97/freecars/FreeCars_isr.c)�� [FreeCars_uart.c](https://github.com/while0l1/stm32f4_modules/blob/master/%E6%A8%A1%E5%9D%97/freecars/FreeCars_uart.c)
- main�����������
  `#include "FreeCars_uart.h"`
- ����FreeCars�ĵ��Թ����õ��Ǵ���1����Ҫ�õ��жϣ��������ԭ���Ĵ���1��ͻ�ˣ����Կ���ɾ�������

- ����Ҫ���͵�������push���ͣ���һ��������ͨ�����ڶ��������ݡ�**ע��** ��������Ϻ���Ҫ����`uSendOnePage();`

- _isr������һ��a�������Կ��ƹ��ܵģ�������main��������һ��ȫ�ֱ���a�����в��ԣ�Ҳ����ɾ�� 