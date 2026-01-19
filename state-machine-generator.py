import re

enum_text = """
typedef enum {
  STATE_NULL,
  STATE_INIT,
  STATE_WAIT,
  STATE_LIGHT,
  STATE_HZ,
  STATE_START,
  STATE_STOP,
  STATE_PULSE,
  STATE_PUMPOUT,
  STATE_PUMPIN,
  STATE_SAMPLERATE,
  STATE_PRINTRATE,
  STATE_STARTSTREAM,
  STATE_STOPSTREAM,
  STATE_SETSECRET,
  STATE_RESET,
  STATE_INFO,
  STATE_HELP,
  NUM_STATES
} stateDef_t;
"""

func_text = """
void sm_state_null(jsonStateData_t &stateData);
void sm_state_init(jsonStateData_t &stateData);
void sm_state_wait(jsonStateData_t &stateData);
void sm_state_light(jsonStateData_t &stateData);
void sm_state_hz(jsonStateData_t &stateData);
void sm_state_start(jsonStateData_t &stateData);
void sm_state_stop(jsonStateData_t &stateData);
void sm_state_pulse(jsonStateData_t &stateData);
void sm_state_pumpout(jsonStateData_t &stateData);
void sm_state_pumpin(jsonStateData_t &stateData);
void sm_state_samplerate(jsonStateData_t &stateData);
void sm_state_printrate(jsonStateData_t &stateData);
void sm_state_start_stream(jsonStateData_t &stateData);
void sm_state_stop_stream(jsonStateData_t &stateData);
void sm_state_set_secret(jsonStateData_t &stateData);
void sm_state_reset(jsonStateData_t &stateData);
void sm_state_info(jsonStateData_t &stateData);
void sm_state_help(jsonStateData_t &stateData);
"""

# ---------------------------------
# Helpers
# ---------------------------------

def strip_comments(text):
    return re.sub(r"//.*", "", text)

def normalize(name):
    return name.lower().replace("_", "")

def state_to_string(state):
    return state.replace("STATE_", "")

# ---------------------------------
# Parse enum states
# ---------------------------------

enum_clean = strip_comments(enum_text)
states = [
    s for s in re.findall(r"\bSTATE_[A-Z0-9_]+\b", enum_clean)
    if s != "NUM_STATES"
]

# ---------------------------------
# Parse function prototypes
# ---------------------------------

func_clean = strip_comments(func_text)
functions = {}

for match in re.findall(r"\bsm_state_[a-z0-9_]+\b", func_clean):
    key = normalize(match.replace("sm_state_", ""))
    functions[key] = match

# ---------------------------------
# Emit StateMachine table
# ---------------------------------

print("StateMachine_t StateMachine[] = {")
for state in states:
    key = normalize(state.replace("STATE_", ""))
    if key in functions:
        print(f"  {{ {state}, {functions[key]} }},")
    else:
        print(f"  // {{ {state}, MISSING_HANDLER }}")
print("};\n")

# ---------------------------------
# Emit function definitions
# ---------------------------------

for key, func in functions.items():
    state_name = key.upper()
    print(f"void {func}(jsonStateData_t &stateData) {{")
    print("  if (lastState != smState) {")
    print("#if DEBUG_STATES == true")
    print(f'    Serial.println(F("state: {state_name}"));')
    print("#endif")
    print("    lastState = smState;")
    print("  }")
    print("  smState = STATE_WAIT;")
    print("}\n")
