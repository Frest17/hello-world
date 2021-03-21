//CODIGO DE EXEMPLO DE USO DE CUDA  PARA TRABALHO DE LP.
//MINI-CALCULADORA DE VETORES
#include <stdio.h>
//#include <stdLib.h>

__global__ void somaV(float* A, float* B, float* C)
{
    int i = threadIdx.x;
    C[i] = A[i] + B[i];
}
__global__ void subV(float* A, float* B, float* C){
	int i = threadIdx.x;
	C[i] = A[i] - B[i];
}

void Menu(){
    printf("------------  Mini-Calculadora de Vetores --------------\n");
	printf("Isso ir� somar cada elemento de dois arrays criados automaticamente--\n");
	printf("C�digo criado para ilustrar o uso de CUDA em opera��es");
	printf("-------- menu --------------------------\n");
    printf("--      1 somar                       --\n");
    printf("--      2 subtrair                    --\n");
    printf("--      3 multiplicar escalar         --\n");
    printf("--      4 imprimir vetores            --\n");
    printf("--------0 sair--------------------------\n");
    printf("----------------------------------------\n");
}

float* criarVetorDevice(int tam){
	//Esse m�todo aloca os vetores em DEVICE.
	float* vetorCuda;
	//Alocando mem�ria no device(gpu):
    cudaMalloc(&vetorCuda, (tam*sizeof(float)));
	return vetorCuda;
}

float* criarVetorHost(int tam, float* vetorCuda){
	//ALOCA VETOR NO HOST E TRANSFERE PRO CUDA CORRESPONDENTE NO DEVICE.
	float* vetorHost;
	vetorHost = malloc(tam*sizeof(float));
	//Inicializa o vetor HOST.
	for(int j = 0; j < tam; j++){
		vetorHost[j] = (j*2+3);
	}
	//Transferindo conte�do dos vetores do host pro device.
	cudaMemcpy(vetorCuda, vetorHost, (tam*sizeof(float)), cudaMemcpyHostToDevice);
	return vetorHost;
}
void deletarVetor(float* a, float* b, float* x, float* y){
	//Libera o espa�o previamente alocado
	cudaFree(b); cudaFree(a);
	free(x); free(y);
}
//METODOS COM KERNEL PARALELO E CUDA
__global__ void somaVetores(int tam, float* deviceA, float* deviceB, float* deviceR)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < tam)
        deviceR[i] = deviceA[i] + deviceB[i];
}
__global__ void subVetores(int tam, float* deviceA, float* deviceB, float* deviceR)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < tam)
        deviceR[i] = deviceA[i] - deviceB[i];
}
__global__ void multVetores(int tam, float* deviceA, float* deviceB, float* deviceR)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < tam)
        deviceR[i] = deviceA[i] * deviceB[i];
}
__global__ void divVetores(int tam, float* deviceA, float* deviceB, float* deviceR)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < tam)
        deviceR[i] = deviceA[i] / deviceB[i];
}
//FIM METODOS COM KERNEL PARALELO E CUDA
int main()
{
	int ative = 1;
	int aux = 0;
	int tam;
	int numBlocos; //numero de blocos
	int numThreads; //numero de threads por bloco
	numThreads = 256;
	printf("Deseja realizar qual operacao?\n");
    while(ative == 1){
        Menu();
        scanf(" %d", &opcao);
		printf("Qual deve ser o tamanho dos vetores?\n");
		scanf(" %d", &tam);
		numBlocos =(tam + numThreads - 1) / numThreads; //Cria��o da quantidade de threads que ser�o usadas.
		//Essa equa��o foi retirada do NVDIA CUDA documentation, mas pode-se usar qualquer quantidade desde que disponivel.
		//Cria��o dos dois vetores no dispisitivo(gpu)
		float* deviceA = criarVetorDevice(tam);
		float* deviceB = criarVetorDevice(tam);
		//Cria��o dos dois vetores no host(cpy)
		float* hostA = criarVetorHost(tam, deviceA);
		float* hostB = criarVetorHost(tam, deviceB);
		
		float* result = malloc(tam*sizeof(float)); //esse guardar� os resultados
		float* deviceR; cudaMalloc(&deviceR, tam*sizeof(float)); //esse guardar� o resultado no device.
        switch(opcao){
        case 1:
            somaVetores<<<numBlocos, numThreads>>>(tam, deviceA, deviceB, deviceR);
			//Metodo kernel para soma com cuda. numBlocos � a qtd blocos de thread. numThreads � a quantidade de threads por bloco.
			cudaMemcpy(result, deviceR, (tam*sizeof(float)), cudaMemcpyDeviceToHost);
			//Transfere da GPU o vetor resultado.
			printf("Resultado:\n");
			for(aux = 0; aux < tam; aux++){
				printf("%f \n", &result[aux]);
			}
            break;
        case 2:
            subVetores<<<numBlocos, numThreads>>>(tam, deviceA, deviceB, deviceR);
			//Metodo kernel para soma com cuda. numBlocos � a qtd blocos de thread. numThreads � a quantidade de threads por bloco.
			cudaMemcpy(result, deviceR, (tam*sizeof(float)), cudaMemcpyDeviceToHost);
			//Transfere da GPU o vetor resultado.
			printf("Resultado:\n");
			for(aux = 0; aux < tam; aux++){
				printf("%f \n", &result[aux]);
			}
            break;
        case 3:
            multVetores<<<numBlocos, numThreads>>>(tam, deviceA, deviceB, deviceR);
			//Metodo kernel para soma com cuda. numBlocos � a qtd blocos de thread. numThreads � a quantidade de threads por bloco.
			cudaMemcpy(result, deviceR, (tam*sizeof(float)), cudaMemcpyDeviceToHost);
			//Transfere da GPU o vetor resultado.
			printf("Resultado:\n");
			for(aux = 0; aux < tam; aux++){
				printf("%f \n", &result[aux]);
			}
            break;
        case 4:
            divVetores<<<numBlocos, numThreads>>>(tam, deviceA, deviceB, deviceR);
			//Metodo kernel para soma com cuda. numBlocos � a qtd blocos de thread. numThreads � a quantidade de threads por bloco.
			cudaMemcpy(result, deviceR, (tam*sizeof(float)), cudaMemcpyDeviceToHost);
			//Transfere da GPU o vetor resultado.
			printf("Resultado:\n");
			for(aux = 0; aux < tam; aux++){
				printf("%f \n", &result[aux]);
			}
            break;
        case 0:
            ative = 0;
            break;
        default:
            printf("* * ERRO: Inseriu comando inexistente * *");
            break;
        }
    }
	deletarVetor(deviceA,deviceB,hostA,hostB); //Desaloca os dois arrays hosts e dois arrays devices alocados.
    cudaFree(devireR); //Desaloca o que guardava o resultado no DEVICE.
	free(result); //Desaloca o que guardava o resultado no HOST.
    return 0;
}