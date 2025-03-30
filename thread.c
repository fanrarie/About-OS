#include<pthread.h>
#include<stdio.h>
#define MAX 100
int arr[MAX];
int result[MAX];
int length;

int getArray()
{
 
  printf("请输入数据个数:");
  scanf("%d", &length);

  if(length > MAX) {
    printf("超出最大长度!");
    return 0;
  }

  printf("请输入%d个数数据:", length);
    for(int i = 0; i < length; i++) 
    {
    scanf("%d", &arr[i]); 
    }
    
    return length;


}

typedef struct 
{
  int start;
  int end;
} SortParams;

void *sortThread1(void *param1)
{
   // 强制转换参数
  SortParams *params = (SortParams*)param1;  

  // 获取排序范围
  int start = params->start;
  int end = params->end;

  // 使用冒泡排序对指定范围内的数据进行排序
  for(int i = start; i < end; i++)
  {
    for(int j = start; j < end; j++) 
    {
      if(arr[j] > arr[j+1])
      {
        int temp = arr[j];
        arr[j] = arr[j+1]; 
        arr[j+1] = temp;
      }
    }
  }

  // 线程函数完成,返回
  pthread_exit(NULL);
 
}
void *sortThread2(void *param2)
{
// 强制转换参数
  SortParams *params = (SortParams*)param2;  

  // 获取排序范围
  int start = params->start;
  int end = params->end;

  // 使用冒泡排序对指定范围内的数据进行排序
  for(int i = start; i < end; i++)
  {
    for(int j = start; j < end; j++) 
    {
      if(arr[j] > arr[j+1])
      {
        int temp = arr[j];
        arr[j] = arr[j+1]; 
        arr[j+1] = temp;
      }
    }
  }

  // 线程函数完成,返回
  pthread_exit(NULL);
  
}

void *mergeThread(void *param) {

  // 不需要param参数
  int i = 0; 
  int j = length/2;
  int k = 0;

  int temp[length];

  while(i < length/2 && j < length) {
    
    if(arr[i] < arr[j]) {
      temp[k++] = arr[i++];
    }
    else {
      temp[k++] = arr[j++]; 
    }

  }

  while(i < length/2) {
    temp[k++] = arr[i++];
  }

  while(j < length) {
    temp[k++] = arr[j++];
  }

  // 拷贝temp回arr
  for(int i=0; i<length; i++) {
    arr[i] = temp[i];
  }

  pthread_exit(NULL);

  return NULL;
 
}

int main()
{
    int length=getArray();
    SortParams param1;
   param1.start = 0;
   param1.end = length/2;
    SortParams param2;
   param2.start = length/2;
   param2.end = length-1;
    pthread_t thread1,thread2,thread3;
    pthread_create(&thread1,NULL,sortThread1,(void*)&param1);
    pthread_create(&thread2,NULL,sortThread2,(void*)&param2);
    pthread_create(&thread3,NULL,mergeThread,NULL);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
   printf("排序结果为:");
        for(int i=0;i<length;i++)
        {
            printf("%d",arr[i]);
            if(i != length-1) printf(" "); 
        }
    
    return 0;

}
