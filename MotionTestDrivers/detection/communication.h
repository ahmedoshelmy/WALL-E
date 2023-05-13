//===================== START: COMMUNICATION.H =====================
//===================== START: COMMUNICATION.H =====================
//===================== START: COMMUNICATION.H =====================
// TODO: fix errors related to spamming
char str_buff[200]; // a buffer for printing
void print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(str_buff, format, args);
    va_end(args);
    Serial.print(str_buff);
}

bool str_equ(const char *c0, const char *c1)
{
    for (int i = 0;; i++)
    {
        if (c0[i] != c1[i])
            return false;
        if (c0[i] == '\0')
            return true;
    }
}

struct CommandArgument
{
    char *name;
    void *data;

    template <typename T>
    bool operator==(const T &v)
    {
        return (*((T *)data) == v);
    }

    bool name_equal(const char *c)
    {
        return str_equ(c, name);
    }

    template <typename T>
    T get()
    {
        return *((T *)data);
    }

    template <typename T>
    T *getPtr()
    {
        return ((T *)data);
    }
};

class Command
{
    char *name = nullptr;
    CommandArgument *args = nullptr;
    int args_size = 0;

    void readUntil(char *buff, int len)
    {
        int i = 0;
        while (len)
        {
            if (Serial.available())
            {
                buff[i] = Serial.read();
                i++;
                len--;
            }
        }
    }

public:
    bool read()
    {
        if (Serial.available())
        {
            int name_size = Serial.read();
            name = (char *)malloc(name_size + 1);
            name[name_size] = '\0';
            readUntil(name, name_size);
            while (Serial.available() == 0)
            {
            }
            args_size = Serial.read();
            int x = args_size;
            args = (CommandArgument *)malloc(args_size * sizeof(CommandArgument));
            while (x--)
            {
                // read an argument
                while (Serial.available() == 0)
                {
                }
                int ns = Serial.read();
                args[args_size - x - 1].name = (char *)malloc(ns + 1);
                args[args_size - x - 1].name[ns] = '\0';
                readUntil(args[args_size - x - 1].name, ns);
                while (Serial.available() == 0)
                {
                }
                int vs = Serial.read();
                args[args_size - x - 1].data = malloc(vs);
                readUntil((char *)args[args_size - x - 1].data, vs);
            }
            return true;
        }
        return false;
    }

    CommandArgument operator[](const char *r)
    {
        for (int i = 0; i < args_size; i++)
        {
            if (str_equ(r, args[i].name))
            {
                return args[i];
            }
        }

        return args[0]; // default
    }

    CommandArgument operator[](int i)
    {
        return args[i];
    }

    bool operator==(const Command c)
    {
        return str_equ(name, c.name);
    }

    bool operator==(const char *c)
    {
        return str_equ(name, c);
    }

    // returns the command string
    char *cmd()
    {
        return name;
    }

    // returns the size of the args array
    int size()
    {
        return args_size;
    }

    ~Command()
    {
        if (name == nullptr)
            return;
        free(name);

        for (int i = 0; i < args_size; i++)
        {
            free(args[i].name);
            free(args[i].data);
        }

        if (args != nullptr)
            free(args);

        name = nullptr;
        args = nullptr;
    }
};
//===================== END: COMMUNICATION.H =====================