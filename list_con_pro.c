#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
typedef struct list_node
{
    int data;
    struct list_node* next;
}Node,*pNode,**ppNode;

pNode list_head = NULL;

pNode CreatNode(int data)
{
    pNode _n = (pNode)malloc(sizeof(Node));
    if(_n == NULL)
    {
        return NULL;
    }
    _n->data = data;
    _n->next = NULL;
    return _n;
}

void Init(ppNode list)
{
    *list = CreatNode(0);
}

void PushFront(pNode list, int data)
{
    assert(list);
    pNode _n = CreatNode(data);
    if(NULL == _n)
    {
        perror("push error!");
        return ;
    }
    _n->next = list->next;
    list->next = _n;
}

void DelNode(pNode del)
{
    assert(del);
    free(del);
}

int IsEmpty(pNode list)
{
    assert(list);
    return list->next == NULL?1:0;
}

void PopFront(pNode list, int* data)
{
    if(!IsEmpty(list))
    {
        pNode del = list->next;
        list->next = del->next;
        *data = del->data;
        DelNode(del);
    }
    else
    {
        printf("list Empty!\n");
    }
}


void Destroy(pNode list)
{
    int data;
    assert(list);
    while(!IsEmpty(list))
    {
        PopFront(list, &data);
    }
    DelNode(list);
}

void ShowList(pNode list)
{
    assert(list);
    pNode cur = list->next;
    while(cur->next)
    {
        printf("%d->",cur->data);
        cur = cur->next;
    }
    printf("\n");
}

pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;

void *Consumer(void *argv)
{
    int data = 0;
    while(1)
    {
        //sleep(1);
        pthread_mutex_lock(&mylock);
        while(IsEmpty(list_head))
        {
           pthread_cond_wait(&mycond, &mylock);//如果是空，释放锁，再等待，再被唤醒，获得锁。
        }
        PopFront(list_head, &data);
        pthread_mutex_unlock(&mylock);
        printf("consumer:%d\n",data);
    }
    return NULL;
}

void *Product(void *argv)
{
    int data = 0;
    while(1)
    {
        // sleep(1);
        usleep(1234);
        data = rand()%1234;
        pthread_mutex_lock(&mylock);
        PushFront(list_head, data);
        pthread_mutex_unlock(&mylock);
        pthread_cond_signal(&mycond);//唤醒等待的线程。
        printf("Product:%d\n",data);
    }
    return NULL;
}

int main()
{
    Init(&list_head);
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, Consumer, NULL);
    pthread_create(&tid2, NULL, Product, NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    Destroy(list_head);
    pthread_mutex_destroy(&mylock);
    pthread_cond_destroy(&mycond);

    return 0;
}

