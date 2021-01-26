using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Spike
{
    public class Entity
    {
        public ulong ID { get; private set; }

        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
        }
    }
}
