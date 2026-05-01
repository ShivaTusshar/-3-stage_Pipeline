# Pipelined RISC Processor

This project contains a Verilog implementation of a pipelined RISC processor. The top-level module (`pipe`) ties together the pipeline stages, handles register file operations (including forwarding and writeback), manages stalls, and interfaces with separate Instruction and Data memories (Harvard Architecture).

## Architecture Overview

The processor implements a pipelined execution model divided into distinct stages to improve instruction throughput.

### Pipeline Stages

1. **IF / ID (Instruction Fetch & Decode):**
   - Handled by the `IF_ID` module.
   - Fetches instructions from the Instruction Memory (IMEM).
   - Decodes instructions into control signals and register operands.
   - Extracts immediate values.

2. **EX (Execute):**
   - Handled by the `execute` module.
   - Performs ALU operations.
   - Computes branch targets and resolves branch conditions.
   - Calculates memory addresses for Load/Store operations.

3. **WB (Writeback / Memory):**
   - Handled by the `wb` module.
   - Interacts with the Data Memory (DMEM).
   - Selects the final result (from ALU or Memory) to write back to the register file.
   - Generates stall signals (`wb_stall`) to freeze earlier stages if needed.

## Register File & Forwarding

The processor includes a standard 32-register set (`x0` to `x31`), where `x0` is hardwired to zero.

To prevent data hazards, the design implements **Data Forwarding**:

- If an instruction in the EX stage requires an operand that is currently being computed in the WB stage, the WB stage forwards the data (`wb_result` or `wb_read_data`) directly into the pipeline, bypassing the register file.
- Data hazards that cannot be resolved via forwarding (e.g., immediate Load-Use hazards) are resolved using pipeline stalls (`stall_read`).

## Memory Interfaces

The processor uses separate ports for Instruction Memory (IMEM) and Data Memory (DMEM) interfaces.

### IMEM Interface

- `inst_mem_address`: 32-bit output address to fetch the instruction.
- `inst_mem_is_valid`: Input signal validating the memory read.
- `inst_mem_read_data`: 32-bit input instruction data.
- `inst_mem_is_ready`: Output signal indicating the processor is ready to fetch.

### DMEM Interface

- `dmem_read_address` / `dmem_write_address`: Addresses for Load/Store operations.
- `dmem_read_data_temp` / `dmem_write_data`: Data bus for moving data in/out of the processor.
- `dmem_read_valid` / `dmem_write_valid`: Handshake signals from memory.
- `dmem_write_byte`: 4-bit byte-enable signal for store operations (supporting word, half-word, or byte stores).

## File Structure

- `pipeline.v`: The top-level module tying everything together.
- `IF_ID.v`: Instruction Fetch and Decode stage.
- `execute.v`: Execution and ALU stage.
- `memory.v`: Shared definitions or memory controllers.
- `wb.v`: Writeback and data memory interfacing stage.

## Simulation and Monitoring

The top-level `pipeline.v` includes a built-in Lab Monitor block for easy debugging during Verilog simulation.

- **Register Write Logging:** It monitors valid writebacks to the register file. If a non-zero register (and not the stack pointer, `x2`) is modified, it logs the simulation time and the resulting value.
- **Automatic Termination:** The simulation automatically finishes (`$finish`) when an empty/null instruction (`32'h00000000`) is fetched (assuming a non-zero PC), signifying the end of the execution stream.

### Example Console Output:

```text
time: 0 ,result = 0
next_pc = 00000000
time: 20 ,result = 15
All instructions are Fetched
```

## Reset and Initialization

Upon asserting the active-low `reset` signal, the Program Counter (PC) is reset to the parameterized `RESET` address (`32'h0000_0000` by default), the register file is completely zeroed out, and the processor begins in a stalled state until reading stabilizes.
