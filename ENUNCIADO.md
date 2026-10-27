# CHECKPOINT FINAL - HANOI ENGINE

## Objetivo
Desenvolver o motor em C de uma Torre de Hanoi integrada a um frontend pronto.

## Obrigatório
- torres = pilhas sobre listas simplesmente encadeadas;
- histórico = lista encadeada;
- malloc/free;
- regras de movimento;
- vitória;
- integração com frontend;
- análise Big-O.

## Proibido
- vetor/array para representar as torres;
- colocar a regra do jogo no JavaScript;
- alterar o contrato de hanoi.h sem autorização.

## Nota (10,0)
- torres encadeadas: 2,0
- push/pop/peek: 1,5
- validação de movimentos: 1,5
- histórico: 1,5
- integração: 1,0
- malloc/free: 1,0
- organização: 0,5
- Big-O: 1,0

## Bônus
Solução automática recursiva usando a mesma função moverDisco().

## Análise de Complexidade (Big-O)

| Função | Complexidade | Justificativa Técnica |
| :--- | :---: | :--- |
| `push()` | **O(1)** | Operação de tempo constante. O nó é inserido diretamente no topo da pilha atualizando apenas o ponteiro da cabeça (`novo_no->proximo = topo`), sem a necessidade de iterar pela estrutura, independentemente do número de discos $n$. |
| `pop()` | **O(1)** | Operação de tempo constante. Consiste apenas em salvar o valor do elemento do topo, redirecionar o ponteiro principal para o nó seguinte (`topo = topo->proximo`) e liberar a memória do nó removido, executando em passos fixos. |
| `peek()` | **O(1)** | Acesso imediato de tempo constante. Retorna o valor contido no nó apontado pelo topo da pilha sem realizar nenhum percorrimento ou alteração na lista encadeada. |
| `torreVazia()` | **O(1)** | Avaliação condicional direta de tempo constante. Apenas verifica se o ponteiro de topo da estrutura é nulo (`topo == NULL`), retornando um booleano de forma imediata. |
| `registrarMovimento()` | **O(m)** | Complexidade linear em função de $m$ (quantidade de movimentos registrados). Como a lista encadeada do histórico possui apenas ponteiro para o início, cada nova inserção exige percorrer sequencialmente $m-1$ nós até alcançar o ponteiro nulo final para anexar o novo nó. |
| `contarMovimentos()` | **O(m)** | Complexidade linear proporcional a $m$. A função precisa iterar nó por nó desde o início da lista de histórico até o final (`ponteiro_atual != NULL`) incrementando um contador interno para contabilizar o total de jogadas efetuadas. |
| `verificarVitoria()` | **O(n)** | Complexidade linear proporcional a $n$ (número total de discos do jogo). A função valida o estado final inspecionando sequencialmente os nós da pilha da Torre de destino (Torre C) para garantir que há exatamente $n$ discos e que estão estritamente ordenados do menor para o maior. |
| `liberarTorre()` | **O(n)** | Complexidade linear em relação ao número $n$ de discos presentes na torre. Para evitar vazamentos de memória (memory leaks), a função percorre sequencialmente a pilha liberando a memória alocada dinamicamente via `free()` nó por nó até a pilha esvaziar. |
| `liberarHistorico()` | **O(m)** | Complexidade linear em relação ao número $m$ de jogadas armazenadas. Percorre recursiva ou iterativamente todos os $m$ nós alocados na lista encadeada do histórico, invocando `free()` em cada elemento individualmente para desalocação completa. |

---

## Pergunta Teórica Obrigatória

**Como adaptar a estrutura de dados do histórico para que a inserção de um novo movimento passe de O(m) para O(1)?**

**Resposta Expandida:**
Atualmente, a inserção no histórico opera em O(m) porque a lista encadeada é mantida apenas com um ponteiro para o nó inicial (`cabeca`). Para inserir um elemento no final da lista (append), o algoritmo é forçado a percorrer todos os $m$ elementos anteriores para localizar o nó cujo campo `proximo` seja `NULL`.

Para otimizar essa operação para tempo constante **O(1)**, basta estender a estrutura descritora do histórico (o header da lista) para armazenar dois ponteiros:
1. `cabeca` (ou `inicio`): aponta para o primeiro movimento do histórico.
2. `cauda` (ou `fim`): aponta diretamente para o último movimento registrado.

Com essa alteração, a inserção de um novo movimento é feita diretamente no nó final apontado por `cauda`:

```c
// Otimização da inserção de O(m) para O(1)
novo_no->proximo = NULL;
if (historico->cauda != NULL) {
    historico->cauda->proximo = novo_no; // Conecta diretamente no fim sem iterar
} else {
    historico->cabeca = novo_no;         // Caso a lista esteja vazia
}
historico->cauda = novo_no;              // Atualiza o ponteiro de fim
