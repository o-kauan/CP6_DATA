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

## Análise Big-O

### Tabela de Complexidade

| Função | Complexidade Esperada | Justificação |
| :--- | :---: | :--- |
| `push()` | O(1) | Insere um nó diretamente no topo da pilha. |
| `pop()` | O(1) | Remove apenas o nó do topo da pilha. |
| `peek()` | O(1) | Acessa o topo diretamente sem percorrer a lista. |
| `torreVazia()` | O(1) | Verifica apenas se o ponteiro do topo é NULL. |
| `registrarMovimento()` | O(m) | Percorre a lista encadeada do histórico até ao final. |
| `contarMovimentos()` | O(m) | Percorre todos os nós da lista de histórico. |
| `verificarVitoria()` | O(n) | Percorre os discos da torre de destino para verificar a ordem. |
| `liberarTorre()` | O(n) | Libera cada nó da pilha de discos. |
| `liberarHistorico()` | O(m) | Libera cada nó da lista do histórico. |

### Pergunta Obrigatória:
Como adaptar o histórico para que a inserção de um novo movimento passe de O(m) para O(1)?

**Resposta:** 
Deve-se adicionar um ponteiro adicional na estrutura do histórico apontando diretamente para o último nó. Dessa forma, a inserção de um novo movimento é feita instantaneamente em O(1) via `cauda->proximo = novo_no`, sem a necessidade de percorrer toda a lista do início ao fim.
